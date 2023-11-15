#include <iostream>
#include <vector>
#include<map>
#include <crow.h>

#include "Game.h"
import utilities;

// https://docs.google.com/document/d/17Up9pbhwRiUGXRCoE4aJbuTiBvuXILxOdLOBuDKdr3E/edit

int main()
{
	std::vector<std::string> chat;

	crow::SimpleApp app;

	// Test route
	CROW_ROUTE(app, "/")([]() {
		return "Test connection succesful\n";
		});


	// Input server controller
	auto& addMessage = CROW_ROUTE(app, "/chat").methods(crow::HTTPMethod::PUT);
	addMessage([&chat](const crow::request& request) {
		auto messagesReceived = utils::split(request.body, "=");
		chat.push_back(utils::decodeMessage(messagesReceived[1]));
		for (const auto& message : chat)
		{
			std::cout << message << '\n';
		}
		return crow::response(200);
		});


	// Output server controller
	CROW_ROUTE(app, "/chat").methods(crow::HTTPMethod::GET)([&chat]() {
		std::vector<crow::json::wvalue> messages;
		for (int i = 0; i < chat.size(); i++)
		{
			messages.push_back(crow::json::wvalue{ {"message", chat[i]} });
		}
		return crow::json::wvalue{ messages };
		});


	app.port(18080).multithreaded().run();

	return 0;
}