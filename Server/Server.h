#pragma once
#include <vector>
#include <crow.h>
#include <map>

#include "utilities.h"
// import utilitiesMod;
#include "utilitiesModTemp.h"
#include "Game.h"
#include "Chat.h"

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

public:
	Server& IPAddress(const std::string& IPAddress);
	Server& Port(uint16_t port);

public:
	void Run();

private:
	Server();

private:
	static Server* s_instance;
	static Server& GetInstance();

private:
	crow::SimpleApp m_app;
	std::map<uint64_t, Chat> m_chats;

	uint16_t m_port;
	std::string m_IPAddress;
	//DBManager& m_db;
};