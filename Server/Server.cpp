#include <iostream>
#include <vector>

#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>

#include "Game.h"

// https://docs.google.com/document/d/17Up9pbhwRiUGXRCoE4aJbuTiBvuXILxOdLOBuDKdr3E/edit

enum Lobby
{
	player_join,
	player_left,
	game_begin
};

int main()
{
	std::vector<std::string> chat;

	crow::SimpleApp app;

	Lobby lobbyState;

	CROW_ROUTE(app, "/")([]() {
		return "Test connection succesful\n";
		});

	CROW_ROUTE(app, "/inputServer/")([&chat](const crow::request& request) {
		char* message = request.url_params.get("message");
		if (!message)
		{
			std::cout << "Failure\n";
			return crow::response(404);
		}
		std::string str_message = std::string{ message };
		chat.push_back(str_message);

		for (const auto& message : chat)
		{
			std::cout << message << '\n';
		}

		return crow::response(200);
		});

	CROW_ROUTE(app, "/outputServer/")([&chat]() {
		std::vector<crow::json::wvalue> messages;
		for (const auto& message : chat)
		{
			messages.push_back(crow::json::wvalue{ {"message", message}, {"1",1} });
		}
		auto val = crow::json::wvalue{ messages };
		return val;
		});

	CROW_ROUTE(app, "/playerJoin/").methods(crow::HTTPMethod::GET)([&lobbyState](const crow::request& request) {
		std::string name = request.url_params.get("name");
		std::string lobbyStateParam = request.url_params.get("lobbyState");
		if (name.empty() || lobbyStateParam.empty())
		{
			return crow::response(404);
		}
		lobbyState = Lobby::player_join;
		return crow::response(200, "Player joined Lobby");
	    });

	app.port(18080).multithreaded().run();

	return 0;
}