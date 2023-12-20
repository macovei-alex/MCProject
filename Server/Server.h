#pragma once

#include <vector>
#include <crow.h>
#include <map>

#include "serverUtils.h"
#include "serverUtilsModTemp.h"
// import serverUtilsMod;
#include "Game.h"
#include "Database.h"
// #include "MockDatabase.h"

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
	crow::SimpleApp m_app;
	std::map<uint64_t, Game> m_games;

	uint16_t m_port;
	std::string m_IPAddress;
	Database m_database;
};