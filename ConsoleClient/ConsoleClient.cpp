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
	const std::string returnCommand = "/b";

	std::string username;
	std::string password;
	std::string repeatPassword;

menu1:
	utils::PrintMenu1();
	uint8_t option = utils::GetInt("Your option: ");
	switch (option)
	{
	case 1:
		bool isSignInCorrect;
		do {
			username = utils::GetString(std::format("Enter your username (or \"{}\" to go back to the menu): ", returnCommand).c_str());
			if (username == "_b")
				goto menu1;
			password = utils::GetString(std::format("Enter your password (or \"{}\" to go back to the menu): ", returnCommand).c_str());
			if (password == "_b")
				goto menu1;
			isSignInCorrect = utils::SignIn(username, password);
			if (!isSignInCorrect)
			{
				isSignInCorrect = false;
				std::cout << "Please try again.\n";
			}
		} while (!isSignInCorrect);
		break;
	case 2:
		bool isSamePassord;
		do {
			isSamePassord = true;
			username = utils::GetString(std::format("Enter your username (or \"{}\" to go back to the menu): ", returnCommand).c_str());
			if (username == returnCommand)
				goto menu1;
			password = utils::GetString(std::format("Enter your password (or \"{}\" to go back to the menu): ", returnCommand).c_str());
			if (password == returnCommand)
				goto menu1;
			repeatPassword = utils::GetString(std::format("Repeat the password (or \"{}\" to go back to the menu): ", returnCommand).c_str());
			if (repeatPassword == returnCommand)
				goto menu1;
			if (password != repeatPassword)
			{
				std::cout << "Passwords do not match. Please try again\n";
				isSamePassord = false;
			}
		} while (!isSamePassord);
		break;
	case 3:
		return 0;
	}

	char answer = 'y';

menu2:
	utils::PrintMenu2();
	option = utils::GetInt("Your option: ");
	uint64_t roomID;
	switch (option)
	{
	case 1:
		do {
			roomID = utils::CreateRoom();
			if (roomID == LONG_MAX)
			{
				std::cout << "Invalid room ID. Do you want to try again? [y/n]\n"
					<< "Your answer: ";
				std::cin >> answer;
			}
		} while (roomID == LONG_MAX && answer == 'y');
		if (answer == 'n')
			goto menu2;
		break;
	case 2:
		bool isGoodConnection;
		do {
			std::cout << "Enter room ID: ";
			roomID = utils::GetInt();
			isGoodConnection = utils::ConnectToRoom(roomID);
			if (!isGoodConnection)
			{
				std::cout << std::format("Do you want to try again? [y/n]\n", roomID)
					<< "Your answer: ";
				std::cin >> answer;
			}
		} while (!isGoodConnection && answer == 'y');
		if(answer == 'n')
			goto menu2;
		break;
	case 3:
		goto menu1;
	case 4:
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