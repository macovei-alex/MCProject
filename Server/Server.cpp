#include "Server.h"

#include <format>

Server::Server() :
	m_app{}, m_chat{}, m_lobbyState{ utils::Lobby::player_join }, m_port{ 0 }
{
	/* Empty */
}

Server& Server::getInstance()
{
	if (!instance)
		instance = new Server();
	return *instance;
}

Server& Server::allHandlers()
{
	this->testHandlers().chatHandlers().roomHandlers();
	return *this;
}

Server& Server::testHandlers()
{
	CROW_ROUTE(m_app, "/")([]() {
		return "Test connection succesful\n";
		});

	return *this;
}

Server& Server::chatHandlers()
{
	// Input server controller
	auto& putMessage = CROW_ROUTE(m_app, "/chat").methods(crow::HTTPMethod::PUT);
	putMessage([this](const crow::request& request) {
		char* dateTime = new char[101];

		const auto informationVector{ std::move(utils::splitToVec(request.body, "&")) };
		std::map<std::string, std::string> urlParamsMap;

		for (const auto& informationExpression : informationVector)
		{
			auto urlParamPair{ std::move(utils::splitToPair(informationExpression, "=")) };
			urlParamsMap.emplace(std::move(urlParamPair));
		}

		utils::Message message{
			std::move(utils::decodeMessage(urlParamsMap["content"])),
			std::move(utils::decodeMessage(urlParamsMap["author"])),
			time(0)
		};
		ctime_s(dateTime, 100, &message.timestamp);
		dateTime[strlen(dateTime) - 1] = '\0';
		this->m_chat.emplace_back(std::move(message));
		for (const auto& message : this->m_chat)
		{
			std::cout << std::format("[{} at {}]: {}\n",
				message.author,
				dateTime,
				message.content
			);
		}
		delete[] dateTime;
		return crow::response(200);
		});


	// Output server controller
	auto& getMessages = CROW_ROUTE(m_app, "/chat").methods(crow::HTTPMethod::GET);
	getMessages([this](const crow::request& request) {
		time_t from = std::stoi(request.url_params.get("from"));
		const std::string senderName{ std::move(request.url_params.get("author")) };
		std::vector<crow::json::wvalue> messages;
		for (int i = this->m_chat.size() - 1; i >= 0 && this->m_chat[i].timestamp >= from; i--)
		{
			if (from == 0 || this->m_chat[i].author != senderName)
				messages.insert(messages.begin(), crow::json::wvalue{
					{"content", this->m_chat[i].content},
					{"author", this->m_chat[i].author},
					{"timestamp", this->m_chat[i].timestamp}
					});
		}
		return crow::json::wvalue{ messages };
		});

	return *this;
}

Server& Server::roomHandlers()
{
	// Player join
	CROW_ROUTE(m_app, "/playerJoin/").methods(crow::HTTPMethod::GET)([this](const crow::request& request) {
		std::string name = request.url_params.get("name");
		std::string lobbyStateParam = request.url_params.get("lobbyState");
		if (name.empty() || lobbyStateParam.empty())
		{
			return crow::response(404);
		}
		this->m_lobbyState = utils::Lobby::player_join;
		return crow::response(200, "Player joined Lobby");
		});

	// Player left
	CROW_ROUTE(m_app, "/playerLeft/").methods(crow::HTTPMethod::GET)([this](const crow::request& request) {
		std::string name = request.url_params.get("name");
		std::string lobbyStateParam = request.url_params.get("lobbyState");
		if (name.empty() || lobbyStateParam.empty())
		{
			return crow::response(404);
		}
		this->m_lobbyState = utils::Lobby::player_left;
		return crow::response(200, "Player left Lobby");
		});

	// Game begin
	CROW_ROUTE(m_app, "/gameBegin/").methods(crow::HTTPMethod::GET)([this](const crow::request& request) {
		std::string name = request.url_params.get("name");
		std::string lobbyStateParam = request.url_params.get("lobbyState");
		if (name.empty() || lobbyStateParam.empty())
		{
			return crow::response(404);
		}
		this->m_lobbyState = utils::Lobby::game_begin;
		return crow::response(200, "Game has begun");
		});

	return *this;
}

Server& Server::port(uint16_t portNumber)
{
	m_port = portNumber;
	return *this;
}

void Server::run()
{
	try
	{
		if (m_port == 0)
			throw std::exception("Port number not set");
		m_app.port(m_port).multithreaded().run();
	}
	catch (std::exception ex)
	{
		std::cout << ex.what() << '\n';
	}
}