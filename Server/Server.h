#pragma once

#include "serverUtils.h"
// import serverUtilsMod;
#include "serverUtilsModTemp.h"
#include "Game.h"

#include <vector>
#include <crow.h>
#include <map>

class Server
{
public:
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
	Server();

public:
	static Server& GetInstance();

private:
	static Server* s_instance;

private:
	crow::SimpleApp m_app;
	std::map<uint64_t, Game> m_games;

	uint16_t m_port;
	std::string m_IPAddress;
	//Database& m_db;
};