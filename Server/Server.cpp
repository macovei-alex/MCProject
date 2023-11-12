#include <iostream>
#include <vector>
#include<map>
#include <crow.h>
#include "Game.h"
// https://docs.google.com/document/d/17Up9pbhwRiUGXRCoE4aJbuTiBvuXILxOdLOBuDKdr3E/edit
std::vector<std::string> split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> result;
	size_t startIndex = 0;

	for (size_t found = str.find(delim); found != std::string::npos; found = str.find(delim, startIndex))
	{
		result.emplace_back(str.begin() + startIndex, str.begin() + found);
		startIndex = found + delim.size();
	}
	if (startIndex != str.size())
		result.emplace_back(str.begin() + startIndex, str.end());
	return result;
}

std::unordered_map<std::string, std::string> parseUrlArgs(const std::string& urlArgs)
{
	if (urlArgs == "") {
		return {};
	}

	std::unordered_map<std::string, std::string> result;
	for (const auto& kvStr : split(urlArgs, "&")) {
		auto kvVector = split(kvStr, "=");
		if (kvVector.size() == 2)
			result[kvVector[0]] = kvVector[1];
	}
	return result;
}
int main()
{
	std::vector<std::string> chat;

	crow::SimpleApp app;


	// Test route
	CROW_ROUTE(app, "/")([]() {
		return "Test connection succesful\n";
		});


	//// Input server route for GET
	//CROW_ROUTE(app, "/inputServer/")([&chat](const crow::request& request) {
	//	char* message = request.url_params.get("message");
	//	if (!message)
	//	{
	//		std::cout << "Failure\n";
	//		return crow::response(404);
	//	}
	//	std::string str_message = std::string{ message };
	//	chat.push_back(str_message);

	//	for (const auto& message : chat)
	//	{
	//		std::cout << message << '\n';
	//	}

	//	return crow::response(200);
	//	});


	// Input server route for PUT
	auto& addMessage = CROW_ROUTE(app, "/inputServer/").methods(crow::HTTPMethod::PUT);
	addMessage([&chat](const crow::request& request) {
		auto messagesReceived = split(request.body, "=");
		chat.push_back(messagesReceived[1]);
		for (const auto& message : chat)
		{
			std::cout << message << '\n';
		}
		return crow::response(200);
		});


	// Outut server route
	CROW_ROUTE(app, "/outputServer/")([&chat]() {
		std::vector<crow::json::wvalue> messages;
		for (int i = 0; i < chat.size(); i++)
		{
			messages.push_back(crow::json::wvalue{ {"message", chat[i]}, {"Id",i} });
		}
		return crow::json::wvalue{ messages };
		});


	app.port(18080).multithreaded().run();

	return 0;
}