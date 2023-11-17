#include <iostream>
#include <vector>
#include<map>
#include <crow.h>
#include <format>

#include "Game.h"
import utilities;

// https://docs.google.com/document/d/17Up9pbhwRiUGXRCoE4aJbuTiBvuXILxOdLOBuDKdr3E/edit

enum Lobby
{
	player_join,
	player_left,
	game_begin
};

int main()
{
	std::vector<std::pair<std::string, std::string>> chat;

	crow::SimpleApp app;

	Lobby lobbyState;

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