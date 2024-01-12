#include "Server.h"

#include <ranges>

Server::Server() :
	m_app{ },
	m_games{ },
	m_port{ 0 },
	m_IPAddress{ "127.0.0.1" },
	m_database{ },
	m_logger{ }
{
	/* empty */
}

Server& Server::AllHandlers()
{
	return (*this)
		.TestHandlers()
		.ChatHandlers()
		.RoomHandlers()
		.AccountHandlers()
		.DrawingHandlers()
		.GameHandlers();
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
		Log("Port not set", Logger::Level::Error);
		return;
	}
	if (m_IPAddress.empty())
	{
		Log("IP Address not set", Logger::Level::Error);
		return;
	}

	Log("Server ready to run");
	m_app.bindaddr(m_IPAddress).port(m_port).multithreaded().run();
}

void Server::Log(const std::string_view& message, Logger::Level level)
{
	if (m_logger)
		m_logger->Log(message, level);
}

Server::~Server()
{
	std::vector<std::pair<uint64_t, std::future<void>>> gameStopActions;

	for (auto& [gameID, game] : std::views::all(m_games))
		gameStopActions.emplace_back(gameID, std::async(std::launch::async, [&game]() { game.Stop(); }));

	for (auto& [gameID, gameStopAction] : gameStopActions)
	{
		Log(std::format("Waiting for game < {} > to stop", gameID));
		gameStopAction.wait();
	}

	Log("All games stopped");

	Log("Server stopped");
}

Server& Server::SetSettingsFromFile(const std::string& filePath)
{
	utils::NavigateToProjectDirectory();

	m_logger = std::make_unique<Logger>("server.log");
	m_database = std::make_unique<db::Database>("database.sqlite");

	Log(m_database->ResetPlayerAccounts().reason);

	enum ServerSetting
	{
		allHandlers,
		chatHandlers,
		roomHandlers,
		drawingHandlers
	};
	static const std::map<std::string, ServerSetting> settingsMap
	{
		{"allHandlers", allHandlers},
		{"chatHandlers", chatHandlers},
		{"roomHandlers", roomHandlers},
		{"drawingHandlers", drawingHandlers}
	};

	std::ifstream file{ filePath };

	if (!file.is_open())
	{
		Log(std::format("Failed to open settings file < {} >", filePath), Logger::Level::Error);
		return *this;
	}

	std::string line;

	std::getline(file, line);
	this->IPAddress(line);
	Log(std::format("IP Address set to < {} >", line));

	std::getline(file, line);
	this->Port(std::stoi(line));
	Log(std::format("Port set to < {} >", line));

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

	Log(std::format("Settings Successfuly set from the settings file < {} >", filePath));
	return *this;
}
