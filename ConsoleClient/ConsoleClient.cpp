#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <memory>
#include <chrono>
#include <sstream>
#include <crow.h>
#include <cpr/cpr.h>

#include "..\Common\constantLiterals.h"
#include "inputHandling.h"

bool listeningThreadGoing = true;
std::string name;
uint64_t roomID;

inline auto DateTimeFromInteger(uint64_t millis)
{
	return std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>
		(std::chrono::duration_cast<std::chrono::seconds>
			(std::chrono::milliseconds{ millis }));
}

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
				chr::time_point dateTime = DateTimeFromInteger(messageTimePointMillis);

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
	std::stringstream putUrl;
	std::cout << "Enter your name: ";
	std::getline(std::cin, name);

	PrintMenu1();
	int option = GetInt();
	std::cin.get();
	switch (option)
	{
	case 1:
		try
		{
			auto response = cpr::Get(
				cpr::Url{ "http://localhost:18080/room/new"},
				cpr::Parameters{ {literals::jsonKeys::roomID, std::to_string(roomID)} }
			);
			if (response.status_code != 200 && response.status_code != 201)
				std::cout << "[Sender] Communication error: " << response.reason << '\n';

			roomID = crow::json::load(response.text)[literals::jsonKeys::roomID].u();
			std::cout << "[Sender] New room with roomID < " << roomID << " > created\n";
		}
		catch (const std::exception& e)
		{
			listeningThreadGoing = false;
			std::cout << e.what();
		}
		break;
	case 2:
		try
		{
			roomID = GetInt();
			std::cin.get();
			auto response = cpr::Get(
				cpr::Url{ "http://localhost:18080/room/connect/" + std::to_string(roomID) }
			);
			if (response.status_code != 200 && response.status_code != 201)
				std::cout << "[Sender] Communication error: " << response.reason << '\n';

			if (response.status_code != 200 && response.status_code != 201)
				throw std::exception(("[Sender] Invalid room ID < " + std::to_string(roomID) + " >").c_str());
			std::cout << "[Sender] Connected to room < " << roomID << " > created\n";
		}
		catch (const std::exception& e)
		{
			listeningThreadGoing = false;
			std::cout << e.what();
		}
		break;
	case 3:
		listeningThreadGoing = false;
		break;
	}

	std::thread listeningThread(listener, roomID);
	putUrl << literals::routes::baseAddress << "/game/chat/" << roomID;

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