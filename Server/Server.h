#pragma once

#include <vector>
#include <crow.h>
#include <map>

#include "Game.h"
#include "Database.h"
#include "constantLiterals.h"
#include "common.h"
#include "..\UtilsDLL\utilsDLL.h"

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
	Server& GameHandlers();

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

	std::unique_ptr<db::Database> m_database;
	std::unique_ptr<Logger> m_logger;
	std::map<uint64_t, Game> m_games;

	const crow::json::wvalue m_errorValue{ { literals::error, literals::emptyCString } };
};