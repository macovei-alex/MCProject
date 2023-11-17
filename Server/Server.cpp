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

enum Lobby
{
	player_join,
	player_left,
	game_begin
};

int main()
{
	std::vector<Message> chat;

	crow::SimpleApp app;

	Lobby lobbyState;

	// Test route

	CROW_ROUTE(app, "/")([]() {
		return "Test connection succesful\n";
		});


	// Input server controller
	auto& putMessage = CROW_ROUTE(app, "/chat").methods(crow::HTTPMethod::PUT);
	putMessage([&chat](const crow::request& request) {
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
			time(0)
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
	auto& getMessages = CROW_ROUTE(app, "/chat").methods(crow::HTTPMethod::GET);
	getMessages([&chat](const crow::request& request) {
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

	// Lobby controller

	// Player join
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

	// Player left
	CROW_ROUTE(app, "/playerLeft/").methods(crow::HTTPMethod::GET)([&lobbyState](const crow::request& request) {
		std::string name = request.url_params.get("name");
		std::string lobbyStateParam = request.url_params.get("lobbyState");
		if (name.empty() || lobbyStateParam.empty())
		{
			return crow::response(404);
		}
		lobbyState = Lobby::player_left;
		return crow::response(200, "Player left Lobby");
		});

	// Game begin
	CROW_ROUTE(app, "/gameBegin/").methods(crow::HTTPMethod::GET)([&lobbyState](const crow::request& request) {
		std::string name = request.url_params.get("name");
		std::string lobbyStateParam = request.url_params.get("lobbyState");
		if (name.empty() || lobbyStateParam.empty())
		{
			return crow::response(404);
		}
		lobbyState = Lobby::game_begin;
		return crow::response(200, "Game has begun");
		});

	app.port(18080).multithreaded().run();

	return 0;
}