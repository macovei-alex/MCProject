#include <iostream>
#include <string>

#include <cpr/cpr.h>
#include <crow.h>

int main()
{
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
	
		response = cpr::Get(
			cpr::Url{ "http://localhost:18080/outputServer/" }
		);

		auto messages = crow::json::load(response.text);
		
		for (const auto& mes : messages)
		{
			std::cout << mes["message"] << "\n";
		}
	}

	return 0;
}