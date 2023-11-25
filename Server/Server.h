#pragma once
#include <vector>
#include <crow.h>

import utilitiesMod;

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
	std::vector<utils::Message> m_chat;
	utils::Lobby m_lobbyState;

	uint16_t m_port;
	std::string m_IPAddress;
	//DBManager& m_db;
};