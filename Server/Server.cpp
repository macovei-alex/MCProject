#include "Server.h"

#include <format>
#include <stack>

#include "..\Common\constantLiterals.h"

Server* Server::s_instance = nullptr;

Server::Server() :
	m_app{ },
	m_chats{ },
	m_port{ 0 },
	m_IPAddress{ "127.0.0.1" }
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

	// Input server controller
	CROW_ROUTE(m_app, literals::routes::game::chatParam).methods(crow::HTTPMethod::PUT)
		([this](const crow::request& request, uint64_t gameID) {

		if (this->m_chats.find(gameID) == this->m_chats.end())
			return crow::response(404, std::format("Invalid game ID < {} >", gameID));
		if (request.body.empty())
			return crow::response(404, "Empty request body");

		auto jsonMap{ utils::ParseRequestBody(request.body) };

		auto contentIterator = jsonMap.find(literals::jsonKeys::message::content);
		auto authorIterator = jsonMap.find(literals::jsonKeys::message::author);
		if (contentIterator == jsonMap.end() || authorIterator == jsonMap.end())
			return crow::response(404, "Invalid parameter keys");

		utils::Message message{
			utils::DecodeMessage(contentIterator->second),
			utils::DecodeMessage(authorIterator->second),
			utils::DateTimeAsInteger(std::chrono::system_clock::now())
		};

		std::cout << std::format("[{} at {}]: {}\n",
			message.author,
			message.timeMilliseconds,
			message.content);

		this->m_chats[gameID].emplace_back(std::move(message));

		return crow::response(200);
			});


	// Output server controller
	CROW_ROUTE(m_app, literals::routes::game::chatParam).methods(crow::HTTPMethod::GET)
		([this](const crow::request& request, uint64_t gameID) {

		static const crow::json::wvalue	errorValue{ std::vector<crow::json::wvalue>{{
			{literals::jsonKeys::message::author, literals::error},
			{literals::jsonKeys::message::content, literals::error},
			{literals::jsonKeys::message::timePoint, "0"}}} };

		if (this->m_chats.find(gameID) == this->m_chats.end())
			return errorValue;

		auto& chat = this->m_chats[gameID];

		if (chat.empty())
			return crow::json::wvalue{ std::vector<crow::json::wvalue>{} };

		uint64_t start;
		std::string author;

		try
		{
			start = std::stoull(request.url_params.get(literals::jsonKeys::message::timePoint));
			author = std::move(request.url_params.get(literals::jsonKeys::message::author));
		}
		catch (std::exception ex)
		{
			std::cerr << ex.what();
			return errorValue;
		}

		std::stack<crow::json::wvalue> messagesStack;
		for (int i = chat.size() - 1; i >= 0 && chat[i].timeMilliseconds >= start; i--)
		{
			if (chat[i].author != author || start == 0)
				messagesStack.emplace(crow::json::wvalue{
					{literals::jsonKeys::message::content, chat[i].content},
					{literals::jsonKeys::message::author, chat[i].author},
					{literals::jsonKeys::message::timePoint, chat[i].timeMilliseconds}
					});
		}

		std::vector<crow::json::wvalue> messagesVector;
		messagesVector.reserve(messagesStack.size());
		while (!messagesStack.empty())
		{
			messagesVector.emplace_back(std::move(messagesStack.top()));
			messagesStack.pop();
		}
		return crow::json::wvalue{ messagesVector };
			});

	return *this;
}

Server& Server::RoomHandlers()
{

	// Create roon controller
	CROW_ROUTE(m_app, literals::routes::room::create).methods(crow::HTTPMethod::GET)
		([this](const crow::request& request) {
		uint64_t newRoomID = 0;
		if (!this->m_chats.empty())
			newRoomID = m_chats.rbegin()->first + 1;

		m_chats.insert({ newRoomID, {} });

		return crow::json::wvalue{ {literals::jsonKeys::room::ID, newRoomID } };
			});


	// Connect to room controller
	CROW_ROUTE(m_app, literals::routes::room::connectParam).methods(crow::HTTPMethod::GET)
		([this](const crow::request& request, uint64_t roomID) {

		if (this->m_chats.find(roomID) == this->m_chats.end())
			return crow::response(404, std::format("Invalid room ID < {} >", roomID));

		return crow::response(200, std::format("Connection to room < {} > successful", roomID));
			});


	/*CROW_ROUTE(m_app, literals::routes::room::disconnectParam).methods(crow::HTTPMethod::GET)
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
	CROW_ROUTE(m_app, literals::routes::sign::in).methods(crow::HTTPMethod::GET)
		([this](const crow::request& request) {

		const char* usernameChar = request.url_params.get(literals::jsonKeys::account::username);
		const char* passwordChar = request.url_params.get(literals::jsonKeys::account::password);
		if (!usernameChar || !passwordChar)
			return crow::response(404, "Invalid parameter keys");

		std::string username{ std::move(request.url_params.get(literals::jsonKeys::account::username)) };
		std::string password{ std::move(request.url_params.get(literals::jsonKeys::account::password)) };

		if (username.empty() || password.empty())
			return crow::response(404, std::format("Invalid username < {} > or password < {} >", username, password));

		// try log into database

		return crow::response(200, std::format("Player logged in as < {} >", username));
			});


	CROW_ROUTE(m_app, literals::routes::sign::up).methods(crow::HTTPMethod::POST)
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

		// try create account into database and log in

		return crow::response(200, std::format("Player logged in as < {} >", username));
			});


	CROW_ROUTE(m_app, literals::routes::sign::out).methods(crow::HTTPMethod::PUT)
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

		// log out player

		return crow::response(200, std::format("Player < {} > logged out", username));
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