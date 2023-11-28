#pragma once
#include <vector>
#include <crow.h>
#include <map>

#include "utilities.h"
// import utilitiesMod;
#include "utilitiesModTemp.h"
#include "Game.h"

class Server
{
public:
	Server(const Server&) = delete;
	Server& operator=(const Server&) = delete;
	~Server() = default;

	Server& AllHandlers();
	Server& TestHandlers();
	Server& ChatHandlers();
	Server& RoomHandlers();
	Server& AccountHandlers();
	Server& DrawingHandlers();

	Server& IPAddress(const std::string& IPAddress);
	Server& Port(uint16_t port);

	Server& SetSettingsFromFile(const std::string& filePath = "serverSettings.txt");

	void Run();

	static Server& GetInstance();

private:
	Server();

	static Server* s_instance;

private:
	crow::SimpleApp m_app;
	std::map<uint64_t, std::vector<utils::Message>> m_chats;

	uint16_t m_port;
	std::string m_IPAddress;
	//DBManager& m_db;
};