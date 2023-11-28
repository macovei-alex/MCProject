#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <memory>
#include <chrono>
#include <sstream>
#include <crow.h>
#include <cpr/cpr.h>

#include "clientUtils.h"
#include "..\Common\constantLiterals.h"

bool listeningThreadGoing = true;

void listener(uint64_t gameID, const std::string& username)
{
	using namespace std::literals::chrono_literals;

	std::stringstream getUrl;
	getUrl << literals::routes::baseAddress << "/game/chat/" << gameID;
	std::cout << std::format("[Listener] Listening to {}\n", getUrl.str());

	uint64_t lastTimeMillis = 0;
	bool serverErrorDetected = false;

	while (listeningThreadGoing)
	{
		try
		{
			auto response = cpr::Get(
				cpr::Url{ getUrl.str() },
				cpr::Parameters{
					{literals::jsonKeys::message::author, username},
					{literals::jsonKeys::message::timePoint, std::to_string(lastTimeMillis)}
				});
			if (response.status_code != 200 && response.status_code != 201)
			{
				if (!serverErrorDetected)
				{
					std::cout << std::format("[Listener] {}\n", response.reason);
					serverErrorDetected = true;
				}
				continue;
			}
			else
				serverErrorDetected = false;

			auto messagesJson = crow::json::load(response.text);
			if (messagesJson.size() != 0)
				lastTimeMillis = messagesJson[messagesJson.size() - 1][literals::jsonKeys::message::timePoint].u() + 1;
			else if (lastTimeMillis == 0)
				lastTimeMillis = utils::NowAsInteger();

			for (auto& messageJson : messagesJson)
			{
				uint64_t messageTimePointMillis = messageJson[literals::jsonKeys::message::timePoint].u();
				auto dateTime = utils::DateTimeFromInteger(messageTimePointMillis);

				std::cout << std::format("[{} at {}]: {}\n",
					std::string{ std::move(messageJson[literals::jsonKeys::message::author].s()) },
					dateTime,
					std::string{ std::move(messageJson[literals::jsonKeys::message::content].s()) }
				);
			}

			std::this_thread::sleep_for(0.5s);
		}
		catch (std::exception exception)
		{
			std::cout << "[Listener] Error detected: " << exception.what() << "\nThe app will close after you enter any character\n";
			listeningThreadGoing = false;
		}
	}
}

int main()
{
	std::string username;
	std::string password;

	utils::PrintMenu1();
	uint8_t option = utils::GetInt("Your option: ");
	switch (option)
	{
	case 1:
		username = utils::GetString("Enter your username: ");
		password = utils::GetString("Enter your password: ");
		if (!utils::SignIn(username, password))
		{
			std::cout << "Press enter to continue...";
			std::cin.get();
			return 1;
		}
		break;
	case 2:
		username = utils::GetString("Enter your username: ");
		password = utils::GetString("Enter your password: ");
		if (!utils::SignIn(username, password))
		{
			std::cout << "Press enter to continue...";
			std::cin.get();
			return 1;
		}
		break;
	case 3:
		return 0;
	}

	utils::PrintMenu2();
	option = utils::GetInt("Your option: ");
	uint64_t roomID;
	switch (option)
	{
	case 1:
		roomID = utils::CreateRoom();
		if (roomID == LONG_MAX)
		{
			std::cout << "Press enter to continue...";
			std::cin.get();
			return 1;
		}
		break;
	case 2:
		std::cout << "Enter room ID: ";
		roomID = utils::GetInt();
		if (!utils::ConnectToRoom(roomID))
		{
			std::cout << "Press enter to continue...";
			std::cin.get();
			return 1;
		}
		break;
	case 3:
		return 0;
	}

	std::thread listeningThread(listener, roomID, username);
	std::stringstream putUrl;
	putUrl << literals::routes::baseAddress << literals::routes::game::chat << '/' << roomID;

	while (listeningThreadGoing)
	{
		try
		{
			std::string message;
			std::getline(std::cin, message);
			auto response = cpr::Put(
				cpr::Url{ putUrl.str() },
				cpr::Payload{
					{literals::jsonKeys::message::author, username},
					{literals::jsonKeys::message::content, message} }
			);
			if (response.status_code != 200 && response.status_code != 201)
				std::cout << std::format("[Sender] {}\n", response.reason);
		}
		catch (const std::exception& e)
		{
			listeningThreadGoing = false;
			std::cout << e.what();
		}
	}

	listeningThread.join();

	return 0;
}