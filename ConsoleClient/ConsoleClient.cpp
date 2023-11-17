#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include <cpr/cpr.h>
#include <crow.h>
#include <crow/json.h>

bool listeningThreadGoing = true;
std::vector<std::pair<std::string, std::string>> clientChat;

void listener()
{
	using namespace std::literals::chrono_literals;
	while (listeningThreadGoing)
	{
		try
		{
			auto response = cpr::Get(
				cpr::Url{ "http://localhost:18080/chat" }
			);
			auto messages = crow::json::load(response.text);
			for (int i = clientChat.size(); i < messages.size(); i++)
			{
				std::string sender = messages[i]["sender"].s();
				std::string message = messages[i]["message"].s();
				clientChat.push_back({sender, message});
				std::cout << std::format("[{}]: {}", sender, message) << "\n";
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
	const std::string name{ "Nume persoana" };
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
				cpr::Payload{ {"sender", name}, {"message", message}}
			);
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