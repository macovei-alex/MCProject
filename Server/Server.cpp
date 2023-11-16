#include <iostream>
#include <vector>
#include<map>
#include <crow.h>
#include <format>

#include "Game.h"
import utilities;

// https://docs.google.com/document/d/17Up9pbhwRiUGXRCoE4aJbuTiBvuXILxOdLOBuDKdr3E/edit

int main()
{
	std::vector<std::pair<std::string, std::string>> chat;

	crow::SimpleApp app;

	// Test route
	CROW_ROUTE(app, "/")([]() {
		return "Test connection succesful\n";
		});


	// Input server controller
	auto& addMessage = CROW_ROUTE(app, "/chat").methods(crow::HTTPMethod::PUT);
	addMessage([&chat](const crow::request& request) {

		const auto informationVector{ std::move(utils::splitToVec(request.body, "&")) };
		std::map<std::string, std::string> informationMap;

		for (const auto& informationExpression : informationVector)
		{
			auto informationPair{ std::move(utils::splitToPair(informationExpression, "=")) };
			informationMap.emplace(std::move(informationPair));
		}

		std::string senderDecoded{ std::move(utils::decodeMessage(informationMap["sender"])) };
		std::string messageDecoded{ std::move(utils::decodeMessage(informationMap["message"])) };
		chat.emplace_back(std::move(std::pair{ senderDecoded, messageDecoded }));
		for (const auto& message : chat)
		{
			std::cout << std::format("[{}]: {}\n", message.first, message.second);
		}
		return crow::response(200);
		});


	// Output server controller
	CROW_ROUTE(app, "/chat").methods(crow::HTTPMethod::GET)([&chat]() {
		std::vector<crow::json::wvalue> messages;
		for (int i = 0; i < chat.size(); i++)
		{
			messages.push_back(crow::json::wvalue{ {"sender", chat[i].first}, {"message", chat[i].second} });
		}
		return crow::json::wvalue{ messages };
		});


	app.port(18080).multithreaded().run();

	return 0;
}