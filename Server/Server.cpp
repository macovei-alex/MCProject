#include <iostream>
#include <vector>

#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>

#include "Game.h"

// https://docs.google.com/document/d/17Up9pbhwRiUGXRCoE4aJbuTiBvuXILxOdLOBuDKdr3E/edit

int main()
{
	std::vector<std::string> chat;

	crow::SimpleApp app;

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

	app.port(18080).multithreaded().run();

	return 0;
}