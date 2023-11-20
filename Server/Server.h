#pragma once
#include <vector>
#include <crow.h>

import utilities;

class Server
{
public:
	Server();
	Server(const Server&) = delete;
	Server& operator=(const Server&) = delete;
	~Server() = default;

	Server& allHandlers();
	Server& testHandlers();
	Server& chatHandlers();
	Server& roomHandlers();

	Server& port(uint16_t port);

	void run();

private:
	crow::SimpleApp m_app;
	std::vector<utils::Message> m_chat;
	utils::Lobby m_lobbyState;

	uint16_t m_port;
};