#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <memory>
#include <chrono>
#include <crow.h>
#include <cpr/cpr.h>

#include "..\Common\jsonKeys.h"

bool listeningThreadGoing = true;
std::string name;

void listener()
{
	using namespace std::literals::chrono_literals;
	namespace chr = std::chrono;

	uint64_t lastTimeMillis = 0;
	std::unique_ptr<char> dateTime{ new char[101] };
	bool serverErrorDetected = false;

	while (listeningThreadGoing)
	{
		try
		{
			auto response = cpr::Get(
				cpr::Url{ "http://localhost:18080/chat" },
				cpr::Parameters{ {keys::message::author, name}, {keys::message::timePoint, std::to_string(lastTimeMillis)} }
			);
			if (response.status_code != 200 && response.status_code != 201)
			{
				if (!serverErrorDetected)
				{
					std::cout << "[Listener] Bad server response\n";
					serverErrorDetected = true;
				}
				continue;
			}
			else
				serverErrorDetected = false;

			auto messagesJson = crow::json::load(response.text);
			if (messagesJson.size() != 0)
				lastTimeMillis = messagesJson[messagesJson.size() - 1][keys::message::timePoint].u() + 1;
			else if (lastTimeMillis == 0)
				lastTimeMillis = chr::duration_cast<chr::milliseconds>
				(chr::system_clock::now().time_since_epoch()).count();

			for (auto& messageJson : messagesJson)
			{
				chr::milliseconds messageTimePointMillis{ messageJson[keys::message::timePoint].u() };
				chr::time_point dateTime = chr::time_point<chr::system_clock, chr::seconds>
					(chr::duration_cast<chr::seconds>
						(chr::milliseconds{ messageTimePointMillis }));
				std::cout << std::format("[{} at {}]: {}\n",
					std::string{ std::move(messageJson[keys::message::author].s()) },
					dateTime,
					std::string{ std::move(messageJson[keys::message::content].s()) }
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
	std::cout << "Enter your name: ";
	std::getline(std::cin, name);

	std::thread listeningThread(listener);

	while (listeningThreadGoing)
	{
		std::string message;
		std::getline(std::cin, message);
		if (message == "q" || !listeningThreadGoing)
		{
			listeningThreadGoing = false;
			break;
		}

		try
		{
			auto response = cpr::Put(
				cpr::Url{ "http://localhost:18080/chat" },
				cpr::Payload{ {{keys::message::author, name}, {keys::message::content, message} } }
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