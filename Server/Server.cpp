#include <iostream>
#include <vector>
#include <map>
#include <crow.h>
#include <format>

#include "Game.h"
import utilities;

// https://docs.google.com/document/d/17Up9pbhwRiUGXRCoE4aJbuTiBvuXILxOdLOBuDKdr3E/edit

struct Message
{
	std::string content;
	std::string author;
	time_t timestamp;

	Message() = default;

	Message(std::string&& content, std::string&& author, time_t timestamp) :
		content{ std::move(content) },
		author{ std::move(author) },
		timestamp{ timestamp }
	{
		/* EMPTY */
	}
};

int main()
{
	std::vector<Message> chat;

	crow::SimpleApp app;

	// Test route
	CROW_ROUTE(app, "/")([]() {
		return "Test connection succesful\n";
		});


	// Input server controller
	auto& addMessage = CROW_ROUTE(app, "/chat").methods(crow::HTTPMethod::PUT);
	addMessage([&chat](const crow::request& request) {
		char* dateTime = new char[101];

		const auto informationVector{ std::move(utils::splitToVec(request.body, "&")) };
		std::map<std::string, std::string> informationMap;

		for (const auto& informationExpression : informationVector)
		{
			auto informationPair{ std::move(utils::splitToPair(informationExpression, "=")) };
			informationMap.emplace(std::move(informationPair));
		}

		Message message{ 
			std::move(utils::decodeMessage(informationMap["content"])), 
			std::move(utils::decodeMessage(informationMap["author"])), 
			std::stoi(informationMap["timestamp"])
		};
		ctime_s(dateTime, 100, &message.timestamp);
		dateTime[strlen(dateTime) - 1] = '\0';
		chat.emplace_back(std::move(message));
		for (const auto& message : chat)
		{
			std::cout << std::format("[{} at {}]: {}\n", 
				message.author, 
				dateTime, 
				message.content
			);
		}
		delete[] dateTime;
		return crow::response(200);
		});


	// Output server controller
	CROW_ROUTE(app, "/chat").methods(crow::HTTPMethod::GET)([&chat](const crow::request& request) {
		time_t from = std::stoi(request.url_params.get("from"));
		std::vector<crow::json::wvalue> messages;
		for (int i = chat.size() - 1; i >= 0 && from <= chat[i].timestamp; i--)
		{
			messages.insert(messages.begin(), crow::json::wvalue{
				{"content", chat[i].content},
				{"author", chat[i].author},
				{"timestamp", chat[i].timestamp}
				});
		}
		return crow::json::wvalue{ messages };
		});


	app.port(18080).multithreaded().run();

	return 0;
}