#include <iostream>
#include <string>
#include<vector>
#include <cpr/cpr.h>
#include <crow.h>
#include<thread>
bool main_tread_stop = false;
std::vector<std::string>ClientChat;
using namespace  std::literals::chrono_literals;
void Request() {
	while (!main_tread_stop)
	{

		auto response = cpr::Get(
			cpr::Url{ "http://localhost:18080/outputServer/" }
		);

		auto messages = crow::json::load(response.text);
		for (int i = ClientChat.size(); i < messages.size(); i++)
		{
			ClientChat.push_back(messages[i]["message"].s());
			std::cout << messages[i]["message"] << "\n";
		}
		std::this_thread::sleep_for(1s);
	}
 }
int main()
{
	std::thread Listener(Request);
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
	main_tread_stop = true;
	Listener.join();

	return 0;
}