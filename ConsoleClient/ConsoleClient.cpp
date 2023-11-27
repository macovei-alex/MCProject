#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <memory>
#include <chrono>
#include <sstream>
#include <crow.h>
#include <cpr/cpr.h>

#include "inputHandling.h"
#include "clientUtils.h"
#include "..\Common\constantLiterals.h"

bool listeningThreadGoing = true;
std::string name;

void listener(uint64_t gameID)
{
	using namespace std::literals::chrono_literals;
	namespace chr = std::chrono;

	std::stringstream getUrl;
	getUrl << literals::routes::baseAddress << "/game/chat/" << gameID;

	std::cout << "[Listener] Listening to " << getUrl.str() << '\n';

	uint64_t lastTimeMillis = 0;
	bool serverErrorDetected = false;

	while (listeningThreadGoing)
	{
		try
		{
			auto response = cpr::Get(
				cpr::Url{ getUrl.str() },
				cpr::Parameters{
					{literals::jsonKeys::message::author, name},
					{literals::jsonKeys::message::timePoint, std::to_string(lastTimeMillis)} });
			if (response.status_code != 200 && response.status_code != 201)
			{
				if (!serverErrorDetected)
				{
					std::cout << "[Listener] Communication error: " << response.reason << '\n';
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
				lastTimeMillis = chr::duration_cast<chr::milliseconds>
				(chr::system_clock::now().time_since_epoch()).count();

			for (auto& messageJson : messagesJson)
			{
				uint64_t messageTimePointMillis = chr::milliseconds{ messageJson[literals::jsonKeys::message::timePoint].u() }.count();
				auto dateTime = DateTimeFromInteger(messageTimePointMillis);

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
	uint64_t roomID;
	std::cout << "Enter your name: ";
	std::getline(std::cin, name);

	PrintMenu1();
	int option = GetInt();
	std::cin.get();
	switch (option)
	{
	case 1:
		roomID = CreateRoom();
		if(roomID == LONG_MAX)
			listeningThreadGoing = false;
		break;
	case 2:
		std::cout << "Enter room ID: ";
		roomID = GetInt();
		std::cin.get();
		if (!ConnectToRoom(roomID))
			listeningThreadGoing = false;
		break;
	case 3:
		listeningThreadGoing = false;
		break;
	}

	std::thread listeningThread(listener, roomID);
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
					{literals::jsonKeys::message::author, name},
					{literals::jsonKeys::message::content, message} }
			);
			if (response.status_code != 200 && response.status_code != 201)
				std::cout << "[Sender] Server connection error detected\n";
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