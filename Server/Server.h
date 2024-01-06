#pragma once

#include <vector>
#include <crow.h>
#include <map>

#include "serverUtils.h"
#include "Game.h"
#include "Database.h"

#include "MockDatabase.h"
// temp

#include "serverUtilsModTemp.h"
// import serverUtilsMod;

#include "../TemporaryLogger/logger.h"
//import logger;

class Server
{
public:
	Server();
	Server(const Server&) = delete;
	Server& operator=(const Server&) = delete;
	~Server() = default;
	Server& SetSettingsFromFile(const std::string& filePath = "serverSettings.txt");

public:
	Server& AllHandlers();
	Server& TestHandlers();
	Server& ChatHandlers();
	Server& RoomHandlers();
	Server& AccountHandlers();
	Server& DrawingHandlers();
	Server& GameSettingsHandlers();

public:
	Server& IPAddress(const std::string& IPAddress);
	Server& Port(uint16_t port);

public:
	void Run();

private:
	void Log(const std::string_view& message, Logger::Level level = Logger::Level::Info);

private:
	crow::SimpleApp m_app;
	uint16_t m_port;
	std::string m_IPAddress;

	db::Database m_database;
	Logger m_logger;
	std::map<uint64_t, Game> m_games;
};