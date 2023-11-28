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

		const auto informationVector{ std::move(utils::SplitToVec(request.body, "&")) };
		std::map<std::string, std::string> urlParamsMap;

		for (const auto& informationExpression : informationVector)
		{
			auto urlParamPair{ std::move(utils::SplitToPair(informationExpression, "=")) };
			urlParamsMap.emplace(std::move(urlParamPair));
		}

		utils::Message message;
		try 
		{
			uint64_t timeMillis = utils::DateTimeAsInteger(std::chrono::system_clock::now());
			 message = utils::Message{
				std::move(utils::DecodeMessage(urlParamsMap[literals::jsonKeys::message::content])),
				std::move(utils::DecodeMessage(urlParamsMap[literals::jsonKeys::message::author])),
				timeMillis
			};
		}
		catch (std::exception ex)
		{
			std::cerr << ex.what();
			return crow::response(404, "Invalid parameter descriptors");
		}

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

		static const crow::json::wvalue	errorValue{ {
			{literals::jsonKeys::message::author, literals::error},
			{literals::jsonKeys::message::content, literals::error},
			{literals::jsonKeys::message::timePoint, std::to_string(0)}} };

		if (this->m_chats.find(gameID) == this->m_chats.end())
			return errorValue;

		auto& chat = this->m_chats[gameID];

		uint64_t start;
		std::string author;
		try
		{
			start = std::stoll(request.url_params.get(literals::jsonKeys::message::timePoint));
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
			if (start == 0 || chat[i].author != author)
				messagesStack.push(crow::json::wvalue{
					{literals::jsonKeys::message::content, chat[i].content},
					{literals::jsonKeys::message::author, chat[i].author},
					{literals::jsonKeys::message::timePoint, chat[i].timeMilliseconds} });
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
		uint64_t newGameID = 0;
		if (!this->m_chats.empty())
			newGameID = m_chats.rbegin()->first + 1;

		m_chats.insert({ newGameID, {} });

		return crow::json::wvalue{ { {literals::jsonKeys::room::roomID, newGameID } } };
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

		std::string username, password;
		try
		{
			username = std::move(request.url_params.get(literals::jsonKeys::account::username));
			password = std::move(request.url_params.get(literals::jsonKeys::account::password));
		}
		catch (const std::exception& ex)
		{
			std::cout << ex.what();
			return crow::response(404, std::format("Invalid username < {} > or password < {} >", username, password));
		}
		if (username.empty() || password.empty())
			return crow::response(404, std::format("Invalid username < {} > or password < {} >", username, password));

		// try log into database

		return crow::response(200, std::format("Player logged in as < {} >", username));
		});


	CROW_ROUTE(m_app, literals::routes::sign::up).methods(crow::HTTPMethod::POST)
		([this](const crow::request& request) {

		const auto informationVector{ std::move(utils::SplitToVec(request.body, "&")) };
		std::map<std::string, std::string> urlParamsMap;

		for (const auto& informationExpression : informationVector)
		{
			auto urlParamPair{ std::move(utils::SplitToPair(informationExpression, "=")) };
			urlParamsMap.emplace(std::move(urlParamPair));
		}

		if(urlParamsMap.find(literals::jsonKeys::account::username) == urlParamsMap.end() 
		|| urlParamsMap.find(literals::jsonKeys::account::password) == urlParamsMap.end())
			return crow::response(404, "Invalid parameter descriptors");

		std::string username, password;
		try
		{
			username = urlParamsMap[literals::jsonKeys::account::username];
			password = urlParamsMap[literals::jsonKeys::account::password];
		}
		catch (std::exception ex)
		{
			std::cout << ex.what();
			return crow::response(404, std::format("Invalid username < {} > or password < {} >", username, password));
		}
		if (username.empty() || password.empty())
			return crow::response(404, std::format("Invalid username < {} > or password < {} >", username, password));

		// try create account into database and log in

		return crow::response(200, std::format("Player logged in as < {} >", username));
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