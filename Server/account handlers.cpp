#include "Server.h"

Server& Server::AccountHandlers()
{
	CROW_ROUTE(m_app, literals::routes::account::signIn).methods(crow::HTTPMethod::Get)
		([this](const crow::request& request) {

		const char* usernameChar = request.url_params.get(literals::jsonKeys::account::username);
		const char* passwordChar = request.url_params.get(literals::jsonKeys::account::password);
		if (!usernameChar || !passwordChar)
			return crow::response{ 404, "Invalid parameter keys" };

		std::string username{ usernameChar };
		std::string password{ passwordChar };

		if (username.empty() || password.empty())
		{
			auto responseMessage{ std::format("Invalid username < {} > or password < {} >", username, password) };
			Log(responseMessage, Logger::Level::Error);
			return crow::response{ 404, responseMessage };
		}

		if (username != "root")
		{
			db::ReturnValue returnValue{ std::move(m_database->SignIn(username, password)) };
			if (!returnValue.success)
			{
				Log(returnValue.reason, Logger::Level::Error);
				return crow::response{ 404, returnValue.reason };
			}
		}

		auto responseMessage{ std::format("Player logged in as < {} >", username) };
		Log(responseMessage, Logger::Level::Info);
		return crow::response{ 200, responseMessage };
			});


	CROW_ROUTE(m_app, literals::routes::account::singUp).methods(crow::HTTPMethod::Post)
		([this](const crow::request& request) {

		if (request.body.empty())
		{
			auto responseMessage{ "Empty request body" };
			Log(responseMessage, Logger::Level::Error);
			return crow::response{ 404, "Empty request body" };
		}

		auto jsonMap{ utils::ParseRequestBody(request.body) };

		auto usernameIterator = jsonMap.find(literals::jsonKeys::account::username);
		auto passwordIterator = jsonMap.find(literals::jsonKeys::account::password);
		if (usernameIterator == jsonMap.end() || passwordIterator == jsonMap.end())
		{
			auto responseMessage{ "Invalid parameter keys" };
			Log(responseMessage, Logger::Level::Error);
			return crow::response{ 404, responseMessage };
		}

		std::string username{ std::move(usernameIterator->second) };
		std::string password{ std::move(passwordIterator->second) };

		if (username.empty() || password.empty())
		{
			auto responseMessage{ std::format("Invalid username < {} > or password < {} >", username, password) };
			Log(responseMessage, Logger::Level::Error);
			return crow::response{ 404, responseMessage };
		}

		db::ReturnValue returnValue{ std::move(m_database->SignUp(username, password)) };
		if (!returnValue.success)
		{
			Log(returnValue.reason, Logger::Level::Error);
			return crow::response(404, returnValue.reason);
		}

		auto responseMessage{ std::format("Player logged in as < {} >", username) };
		Log(responseMessage, Logger::Level::Info);
		return crow::response(200, responseMessage);
			});


	CROW_ROUTE(m_app, literals::routes::account::signOut).methods(crow::HTTPMethod::Put)
		([this](const crow::request& request) {

		if (request.body.empty())
		{
			auto responseMessage{ "Empty request body" };
			Log(responseMessage, Logger::Level::Error);
			return crow::response{ 404, responseMessage };
		}

		auto jsonMap{ utils::ParseRequestBody(request.body) };

		auto usernameIterator{ jsonMap.find(literals::jsonKeys::account::username) };
		if (usernameIterator == jsonMap.end())
		{
			auto responseMessage{ "Invalid parameter keys" };
			Log(responseMessage, Logger::Level::Error);
			return crow::response{ 404, responseMessage };
		}

		std::string username{ std::move(usernameIterator->second) };

		if (username.empty())
		{
			auto responseMessage{ std::format("Invalid username < {} >", username) };
			Log(responseMessage, Logger::Level::Error);
			return crow::response{ 404, responseMessage };
		}

		db::ReturnValue returnValue{ std::move(m_database->SignOut(username)) };
		if (!returnValue.success)
		{
			Log(returnValue.reason, Logger::Level::Error);
			return crow::response{ 404, returnValue.reason };
		}

		for (auto& game : m_games)
			game.second.RemovePlayer(username);

		Log(std::format("Player < {} > logged out", username), Logger::Level::Info);
		return crow::response{ 200, std::format("Player < {} > logged out", username) };
			});

	Log("Account handlers set");
	return *this;
}