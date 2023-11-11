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

		auto response = cpr::Get(
			cpr::Url{ "http://localhost:18080/inputServer/" },
			cpr::Parameters{ {"message", message} }
		);
	}

	return 0;
}