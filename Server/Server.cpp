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
			std::cout << "Teapa cumetre\n";
			return crow::response(404);
		}
		chat.push_back(std::string{ message });

		for (const auto& message : chat) {
			std::cout << message << '\n';
		}

		return crow::response(200);
		});

	app.port(18080).multithreaded().run();

	return 0;
}