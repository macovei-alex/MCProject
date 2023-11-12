#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include <cpr/cpr.h>
#include <crow.h>

bool mainThreadGoing = true;
std::vector<std::string> clientChat;

void serverOutputHandler()
{
	using namespace  std::literals::chrono_literals;
	while (mainThreadGoing)
	{
		auto response = cpr::Get(
			cpr::Url{ "http://localhost:18080/outputServer/" }
		);
		auto messages = crow::json::load(response.text);
		for (int i = clientChat.size(); i < messages.size(); i++)
		{
			clientChat.push_back(messages[i]["message"].s());
			std::cout << messages[i]["message"] << "\n";
		}

		std::this_thread::sleep_for(1s);
	}
 }

int main()
{
	std::thread listener(serverOutputHandler);

	while (true)
	{
		std::string message;
	    std::getline(std::cin, message);
		if (message == "q")
			break;

		auto response = cpr::Put(
			cpr::Url{ "http://localhost:18080/inputServer" },
			cpr::Payload{ {"message", message} }
		);
	}

	mainThreadGoing = true;
	listener.join();

	return 0;
}