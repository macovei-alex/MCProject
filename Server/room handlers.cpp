#include "Server.h"

Server& Server::RoomHandlers()
{
	CROW_ROUTE(m_app, literals::routes::game::create).methods(crow::HTTPMethod::Get)
		([this](const crow::request& request) {

		uint64_t newGameID{ 0 };
		if (!this->m_games.empty())
			newGameID = m_games.rbegin()->first + 1;

		m_games.emplace(newGameID, Game{});

		Log(std::format("New room created with ID < {} >", newGameID), Logger::Level::Info);
		return crow::json::wvalue{ {literals::jsonKeys::game::ID, newGameID } };
			});


	CROW_ROUTE(m_app, literals::routes::game::connect::param).methods(crow::HTTPMethod::Get)
		([this](const crow::request& request, uint64_t gameID) {

		auto gameIt{ this->m_games.find(gameID) };
		if (gameIt == m_games.end())
		{
			auto responseMessage{ std::format("Invalid room id < {} >", gameID) };
			Log(responseMessage, Logger::Level::Info);
			return crow::response{ 404, responseMessage };
		}

		Game& game{ gameIt->second };
		try
		{
			if (true) // TODO: Check game state
			{
				std::lock_guard<std::mutex> lock{ game.GetPlayersMutex() };
				game.AddPlayer(Player{ request.url_params.get(literals::jsonKeys::account::username) });
			}
		}
		catch (const std::exception& exception)
		{
			Log(exception.what(), Logger::Level::Error);
			return crow::response{ 404, exception.what() };
		}

		auto responseMessage{ std::format("Connection to room < {} > successful", gameID) };
		Log(responseMessage, Logger::Level::Info);
		return crow::response{ 200, responseMessage };
			});


	CROW_ROUTE(m_app, literals::routes::game::disconnect::param).methods(crow::HTTPMethod::Get)
		([this](const crow::request& request, uint64_t roomID) {

		auto gameIt{ m_games.find(roomID) };
		if (gameIt == m_games.end())
		{
			auto responseMessage{ std::format("Invalid room ID < {} >", roomID) };
			Log(responseMessage, Logger::Level::Error);
			return crow::response{ 404, responseMessage };
		}

		try
		{
			std::string username{ request.url_params.get(literals::jsonKeys::account::username) };
			gameIt->second.RemovePlayer(username);
		}
		catch (const std::exception& exception)
		{
			Log(exception.what(), Logger::Level::Error);
			return crow::response{ 404, exception.what() };
		}


		return crow::response{ 200, "Player left Lobby" };
			});

	Log("Room handlers set");
	return *this;
}