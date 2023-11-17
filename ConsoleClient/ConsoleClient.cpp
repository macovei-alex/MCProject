#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <memory>
#include <crow.h>
#include <cpr/cpr.h>

#define CRT_SECURE_NO_WARNINGS

bool listeningThreadGoing = true;
std::string name;

void listener()
{
	using namespace std::literals::chrono_literals;

	time_t lastTimestamp = 0;
	std::unique_ptr<char> dateTime{ new char[101] };
	bool serverErrorDetected = false;

	while (listeningThreadGoing)
	{
		try
		{
			auto response = cpr::Get(
				cpr::Url{ "http://localhost:18080/chat" },
				cpr::Parameters{ {"from", std::to_string(lastTimestamp)}, {"senderName", name} }
			);
			if (response.status_code != 200 && response.status_code != 201)
			{
				if (!serverErrorDetected)
				{
					std::cout << "[Listener] Server connection error detected\n";
					serverErrorDetected = true;
				}
				continue;
			}
			else
				serverErrorDetected = false;

			auto messagesJson = crow::json::load(response.text);
			for (auto& messageJson : messagesJson)
			{
				time_t messageTimestamp = static_cast<time_t>(messageJson["timestamp"].i());
				ctime_s(dateTime.get(), 100, &messageTimestamp);
				dateTime.get()[strlen(dateTime.get()) - 1] = '\0';
				std::cout << std::format("[{} at {}]: {}\n",
					std::string{ std::move(messageJson["author"].s()) },
					dateTime.get(),
					std::string{ std::move(messageJson["content"].s()) }
				);
			}
			if (messagesJson.size() != 0)
				lastTimestamp = static_cast<time_t>(messagesJson[messagesJson.size() - 1]["timestamp"].i()) + 1;
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
				cpr::Payload{ {{"author", name}, {"content", message} } }
			);
			if(response.status_code != 200 && response.status_code != 201)
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