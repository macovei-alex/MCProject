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
	void run();

private:
	const uint16_t k_port = 18080;

private:
	crow::SimpleApp m_app;
	std::vector<utils::Message> m_chat;
	utils::Lobby m_lobbyState;
};