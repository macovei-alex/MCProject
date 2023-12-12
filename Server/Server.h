#pragma once
#include <vector>
#include <crow.h>
#include <map>

#include "serverUtils.h"
// import serverUtilsMod;
#include "serverUtilsModTemp.h"
#include "Game.h"
#include "Chat.h"
#include "Image.h"

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

public:
	static Server& GetInstance();

private:
	static Server* s_instance;

private:
	crow::SimpleApp m_app;
	std::map<uint64_t, Chat> m_chats;
	std::map<uint64_t, Image> m_images;

	uint16_t m_port;
	std::string m_IPAddress;
	//Database& m_db;
};