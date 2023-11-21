#pragma once
#include <vector>
#include <crow.h>

import utilities;

class Server
{
public:
	Server(const Server&) = delete;
	Server& operator=(const Server&) = delete;
	~Server() = default;

	Server& allHandlers();
	Server& testHandlers();
	Server& chatHandlers();
	Server& roomHandlers();
	Server& drawingHandlers();

	Server& IPAddress(const std::string& IP);
	Server& port(uint16_t port);

	void run();

	static Server& getInstance();

private:
	Server();

	static Server* instance;

private:
	crow::SimpleApp m_app;
	std::vector<utils::Message> m_chat;
	utils::Lobby m_lobbyState;

	uint16_t m_port;
	std::string m_IPAddress;
};