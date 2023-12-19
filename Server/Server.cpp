#include "Server.h"

#include "../Common/constantLiterals.h"

#include <format>
#include <stack>

Server* Server::s_instance = nullptr;

Server::Server() :
	m_app{ },
	m_games{ },
	m_port{ 0 },
	m_IPAddress{ "127.0.0.1" },
	m_database{ "database.sqlite" }
{
	/* empty */
}

Server& Server::GetInstance()
{
	if (!s_instance)
		s_instance = new Server();
	return *s_instance;
}

Server& Server::AllHandlers()
{
	this->TestHandlers().ChatHandlers().RoomHandlers().AccountHandlers().DrawingHandlers();
	return *this;
}

Server& Server::TestHandlers()
{
	CROW_ROUTE(m_app, literals::routes::test)([]() {
		return "Test connection succesful\n";
		});

	return *this;
}

Server& Server::ChatHandlers()
{
	CROW_ROUTE(m_app, literals::routes::game::chat::param).methods(crow::HTTPMethod::Put)
		([this](const crow::request& request, uint64_t gameID) {

		auto gameIt = this->m_games.find(gameID);
		if (gameIt == this->m_games.end())
			return crow::response(404, std::format("Invalid room ID < {} >", gameID));

		if (request.body.empty())
			return crow::response(404, "Empty request body");

		auto& chat = gameIt->second.GetChat();

		auto jsonMap{ utils::ParseRequestBody(request.body) };

		auto contentIterator = jsonMap.find(literals::jsonKeys::message::content);
		auto authorIterator = jsonMap.find(literals::jsonKeys::message::author);
		if (contentIterator == jsonMap.end() || authorIterator == jsonMap.end())
			return crow::response(404, "Invalid parameter keys");

		utils::Message message{
			utils::DecodeMessage(contentIterator->second),
			utils::DecodeMessage(authorIterator->second),
			utils::DateTimeAsInteger(std::chrono::system_clock::now()) };

		std::cout << std::format("[{} at {}]: {}\n", message.author, message.timeMilliseconds, message.content);
		chat.Emplace(std::move(message));

		return crow::response(200);
			});


	CROW_ROUTE(m_app, literals::routes::game::chat::param).methods(crow::HTTPMethod::Get)
		([this](const crow::request& request, uint64_t gameID) {

		static const crow::json::wvalue	errorValue{ crow::json::wvalue::list{{literals::error, literals::emptyCString}} };

		auto gameIt = this->m_games.find(gameID);
		if (gameIt == this->m_games.end())
			return errorValue;

		const auto& chat = gameIt->second.GetChat();

		if (chat.Empty())
			return crow::json::wvalue{ crow::json::wvalue::list{} };

		uint64_t start;
		std::string author;
		try
		{
			if (char* startChar = request.url_params.get(literals::jsonKeys::message::timestamp);
				startChar != nullptr)
				start = std::stoull(startChar);
			else
				throw std::exception("Timestamp key not found");

			if (char* authorChar = request.url_params.get(literals::jsonKeys::message::author);
				authorChar != nullptr)
				author = std::string{ authorChar };
			else
				throw std::exception("Author key not found");
		}
		catch (const std::exception& ex)
		{
			std::cerr << ex.what() << '\n';
			return errorValue;
		}

		return crow::json::wvalue{ chat.GetMessagesOrderedJsonList(start, author) };
			});

	return *this;
}

Server& Server::RoomHandlers()
{
	CROW_ROUTE(m_app, literals::routes::game::create).methods(crow::HTTPMethod::Get)
		([this](const crow::request& request) {
		uint64_t newGameID = 0;
		if (!this->m_games.empty())
			newGameID = m_games.rbegin()->first + 1;

		m_games.emplace(newGameID, Game());

		auto& game = m_games[newGameID];

		game.GetImage().AddUpdate(utils::img::Update{ utils::img::Point{ 0, 0, utils::img::Color{ 0x0009A2 }}, utils::DateTimeAsInteger(std::chrono::system_clock::now()) });
		game.GetImage().AddUpdate(utils::img::Update{ utils::img::Point{ 1, 2, utils::img::Color{ 0xE00784 } }, utils::DateTimeAsInteger(std::chrono::system_clock::now()) });
		game.GetImage().AddUpdate(utils::img::Update{ utils::img::Point{ 5, 0, utils::img::Color{ 0xAB02F5 } }, utils::DateTimeAsInteger(std::chrono::system_clock::now()) });

		return crow::json::wvalue{ {literals::jsonKeys::game::ID, newGameID } };
			});


	CROW_ROUTE(m_app, literals::routes::game::connectParam).methods(crow::HTTPMethod::Get)
		([this](const crow::request& request, uint64_t gameID) {

		auto gameIt = this->m_games.find(gameID);
		if (gameIt == m_games.end())
			return crow::response(404, std::format("Invalid room ID < {} >", gameID));

		return crow::response(200, std::format("Connection to room < {} > successful", gameID));
			});


	/*CROW_ROUTE(m_app, literals::routes::game::disconnectParam).methods(crow::HTTPMethod::Get)
		([this](const crow::request& request, uint64_t) {
		std::string name = request.url_params.get("name");
		std::string lobbyStateParam = request.url_params.get("lobbyState");
		if (name.empty() || lobbyStateParam.empty())
		{
			return crow::response(404);
		}
		return crow::response(200, "Player left Lobby");
		});*/

	return *this;
}

Server& Server::AccountHandlers()
{
	CROW_ROUTE(m_app, literals::routes::account::signIn).methods(crow::HTTPMethod::Get)
		([this](const crow::request& request) {

		const char* usernameChar = request.url_params.get(literals::jsonKeys::account::username);
		const char* passwordChar = request.url_params.get(literals::jsonKeys::account::password);
		if (!usernameChar || !passwordChar)
			return crow::response(404, "Invalid parameter keys");

		std::string username{ usernameChar };
		std::string password{ passwordChar };

		if (username.empty() || password.empty())
			return crow::response(404, std::format("Invalid username < {} > or password < {} >", username, password));

		/*db::ReturnValue returnValue = m_database.SignIn(username, password);
		if (!returnValue.success)
			return crow::response(404, returnValue.reason);*/

		return crow::response(200, std::format("Player logged in as < {} >", username));
			});


	CROW_ROUTE(m_app, literals::routes::account::singUp).methods(crow::HTTPMethod::Post)
		([this](const crow::request& request) {

		if (request.body.empty())
			return crow::response(404, "Empty request body");

		auto jsonMap{ utils::ParseRequestBody(request.body) };

		auto usernameIterator = jsonMap.find(literals::jsonKeys::account::username);
		auto passwordIterator = jsonMap.find(literals::jsonKeys::account::password);
		if (usernameIterator == jsonMap.end() || passwordIterator == jsonMap.end())
			return crow::response(404, "Invalid parameter keys");

		std::string username{ std::move(usernameIterator->second) };
		std::string password{ std::move(passwordIterator->second) };

		if (username.empty() || password.empty())
			return crow::response(404, std::format("Invalid username < {} > or password < {} >", username, password));

		/*db::ReturnValue returnValue = m_database.SignUp(username, password);
		if (!returnValue.success)
			return crow::response(404, returnValue.reason);*/

		return crow::response(200, std::format("Player logged in as < {} >", username));
			});


	CROW_ROUTE(m_app, literals::routes::account::signOut).methods(crow::HTTPMethod::Put)
		([this](const crow::request& request) {

		if (request.body.empty())
			return crow::response(404, "Empty request body");

		auto jsonMap{ utils::ParseRequestBody(request.body) };

		auto usernameIterator = jsonMap.find(literals::jsonKeys::account::username);
		if (usernameIterator == jsonMap.end())
			return crow::response(404, "Invalid parameter keys");

		std::string username{ std::move(usernameIterator->second) };

		if (username.empty())
			return crow::response(404, std::format("Invalid username < {} >", username));

		/*db::ReturnValue returnValue = m_database.SignOut(username);
		if (!returnValue.success)
			return crow::response(404, returnValue.reason);*/

		return crow::response(200, std::format("Player < {} > logged out", username));
			});

	return *this;
}

Server& Server::DrawingHandlers()
{
	CROW_ROUTE(m_app, literals::routes::game::draw::updatesParam).methods(crow::HTTPMethod::Get)
		([this](const crow::request& request, uint64_t gameID) {

		static const crow::json::wvalue errorValue{ crow::json::wvalue::list{{literals::error, literals::emptyCString}} };

		uint64_t timestamp;

		try
		{
			if (char* timestampChar = request.url_params.get(literals::jsonKeys::draw::timestamp);
				timestampChar != nullptr)
				timestamp = std::stoull(timestampChar);
			else
				throw std::exception("Timestamp key not found");
		}
		catch (const std::exception& ex)
		{
			std::cerr << ex.what() << '\n';
			return errorValue;
		}

		auto updates = m_games[gameID].GetImage().GetUpdatesJsonAfter(timestamp);
		return updates;
			});


	CROW_ROUTE(m_app, literals::routes::game::draw::updatesParam).methods(crow::HTTPMethod::Put)
		([this](const crow::request& request, uint64_t gameID) {

		if (request.body.empty())
			return crow::response(404, "empty request body");

		auto requestBody = crow::json::load(request.body);
		for (const auto& jsonpoint : requestBody)
		{
			utils::img::Point point{
				jsonpoint[literals::jsonKeys::draw::pointX].i(),
				jsonpoint[literals::jsonKeys::draw::pointY].i(),
				jsonpoint[literals::jsonKeys::draw::color].i() };

			m_games[gameID].GetImage().AddUpdate(utils::img::Update{ utils::img::Point{point.x, point.y, point.color}, utils::DateTimeAsInteger(std::chrono::system_clock::now()) });
		}

		return crow::response(200);
			});


	return *this;
}

Server& Server::GameSettingsHandlers()
{
	CROW_ROUTE(m_app, literals::routes::game::settings::param).methods(crow::HTTPMethod::Get)
		([this](const crow::request& request, uint64_t gameID) {

		static const crow::json::wvalue errorValue{ {literals::error, literals::emptyCString} };

		if (m_games.find(gameID) == m_games.end())
			return errorValue;

		return crow::json::wvalue{
			{literals::jsonKeys::settings::drawTime, m_games[gameID].GetGameSettings().GetDrawTime()},
			{literals::jsonKeys::settings::roundCount, m_games[gameID].GetGameSettings().GetRoundCount()},
			{literals::jsonKeys::settings::chooseWordOptionCount, m_games[gameID].GetGameSettings().GetChooseWordOptionCount()} };
			});


	CROW_ROUTE(m_app, literals::routes::game::settings::param).methods(crow::HTTPMethod::Put)
		([this](const crow::request& request, uint64_t gameID) {

		if (request.body.empty())
			return crow::response(404, "Empty request body");

		if (m_games.find(gameID) == m_games.end())
			return crow::response(404, std::format("Invalid game ID < {} >", gameID));

		auto& game = m_games[gameID];

		auto jsonMap{ utils::ParseRequestBody(request.body) };

		try
		{
			uint64_t drawTime = std::stoi(jsonMap.find(literals::jsonKeys::settings::drawTime)->second);
			game.GetGameSettings().SetDrawTime(drawTime);

			int64_t roundCount = std::stoi(jsonMap.find(literals::jsonKeys::settings::roundCount)->second);
			game.GetGameSettings().SetRoundCount(roundCount);

			int64_t chooseWordOptionCount = std::stoi(jsonMap.find(literals::jsonKeys::settings::chooseWordOptionCount)->second);
			game.GetGameSettings().SetDrawTime(chooseWordOptionCount);
		}
		catch (const std::exception& exception)
		{
			std::cerr << exception.what() << '\n';
			return crow::response(404, "Invalid parameter values");
		}

		return crow::response(200);
			});

	return *this;
}

Server& Server::IPAddress(const std::string& IPAddress)
{
	m_IPAddress = IPAddress;
	return *this;
}

Server& Server::Port(uint16_t portNumber)
{
	m_port = portNumber;
	return *this;
}

void Server::Run()
{
	if (m_port == 0)
	{
		std::cerr << "Port not set\n";
		return;
	}
	if (m_IPAddress.empty())
	{
		std::cerr << "IP Address not set";
		return;
	}

	m_app.bindaddr(m_IPAddress).port(m_port).multithreaded().run();
}

Server& Server::SetSettingsFromFile(const std::string& filePath)
{
	utils::NavigateToProjectDirectory();
	enum ServerSetting
	{
		allHandlers,
		chatHandlers,
		roomHandlers,
		drawingHandlers
	};
	const std::map<std::string, ServerSetting> settingsMap
	{
		{"allHandlers", allHandlers},
		{"chatHandlers", chatHandlers},
		{"roomHandlers", roomHandlers},
		{"drawingHandlers", drawingHandlers}
	};

	std::ifstream file{ filePath };
	std::string line;

	std::getline(file, line);
	this->IPAddress(line);
	std::getline(file, line);
	this->Port(std::stoi(line));

	while (std::getline(file, line))
	{
		ServerSetting setting = settingsMap.find(line)->second;
		switch (setting)
		{
		case allHandlers:
			this->AllHandlers();
			return *this;
		case chatHandlers:
			this->ChatHandlers();
			break;
		case roomHandlers:
			this->RoomHandlers();
			break;
		case drawingHandlers:
			this->DrawingHandlers();
			break;
		}
	}
	return *this;
}