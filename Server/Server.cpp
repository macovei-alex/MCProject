#include "Server.h"

#include <format>
#include <map>

#include "utilities.h"

Server* Server::s_instance = nullptr;

Server::Server() :
	m_app{},
	m_chat{},
	m_lobbyState{ utils::Lobby::player_join },
	m_port{ 0 },
	m_IPAddress{ "127.0.0.1" }
{
	/* Empty */
}

Server& Server::GetInstance()
{
	if (!s_instance)
		s_instance = new Server();
	return *s_instance;
}

Server& Server::AllHandlers()
{
	this->TestHandlers().ChatHandlers().RoomHandlers().DrawingHandlers();
	return *this;
}

Server& Server::TestHandlers()
{
	CROW_ROUTE(m_app, "/")([]() {
		return "Test connection succesful\n";
		});

	return *this;
}

Server& Server::ChatHandlers()
{
	// Input server controller
	auto& putMessage = CROW_ROUTE(m_app, "/chat").methods(crow::HTTPMethod::PUT);
	putMessage([this](const crow::request& request) {
		char* dateTime = new char[101];

		const auto informationVector{ std::move(utils::SplitToVec(request.body, "&")) };
		std::map<std::string, std::string> urlParamsMap;

		for (const auto& informationExpression : informationVector)
		{
			auto urlParamPair{ std::move(utils::SplitToPair(informationExpression, "=")) };
			urlParamsMap.emplace(std::move(urlParamPair));
		}

		utils::Message message{
			std::move(utils::DecodeMessage(urlParamsMap["content"])),
			std::move(utils::DecodeMessage(urlParamsMap["author"])),
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

Server& Server::RoomHandlers()
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

Server& Server::DrawingHandlers()
{
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
	try
	{
		if (m_port == 0)
			throw std::exception("Port number not set");
		if (m_IPAddress.empty())
			throw std::exception("IP Address not set");
		m_app.bindaddr(m_IPAddress).port(m_port).multithreaded().run();
	}
	catch (std::exception ex)
	{
		std::cout << ex.what() << '\n';
	}
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
			break;
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