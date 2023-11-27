#include "clientUtils.h"

#include <iostream>
#include <format>
#include <cpr/cpr.h>
#include <crow.h>

#include "..\Common\constantLiterals.h"

uint8_t utils::GetInt()
{
	uint64_t input;
	std::cin >> input;
	std::cin.get();
	return input;
}

std::string&& utils::GetString()
{
	std::string input;
	std::getline(std::cin, input);
	return std::move(input);
}

void utils::PrintMenu1()
{
	std::cout << "Options:\n"
		<< "\t1. Create a new room\n"
		<< "\t2. Join an existing room\n"
		<< "\t3. Exit\n"
		<< "Your option: ";
}

chr::time_point<chr::system_clock, chr::seconds> utils::DateTimeFromInteger(uint64_t millis)
{
	return chr::time_point<chr::system_clock, chr::seconds>
		(chr::duration_cast<chr::seconds>
			(chr::milliseconds{ millis }));
}

uint64_t utils::NowAsInteger()
{
	return chr::duration_cast<chr::milliseconds>
		(chr::system_clock::now().time_since_epoch()).count();
}

uint64_t utils::CreateRoom()
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::room::create;

		auto response = cpr::Get(cpr::Url{ url.str() });

		if (response.status_code != 200 && response.status_code != 201)
			std::cout << std::format("[Sender] Communication error: {}\n", response.reason);

		uint64_t roomID = crow::json::load(response.text)[literals::jsonKeys::roomID].u();
		std::cout << std::format("[Sender] New room with roomID < {} > created\n", roomID);
		return roomID;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
		return LONG_MAX;
	}
}

bool utils::ConnectToRoom(uint64_t roomID)
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::room::connect << '/' << roomID;

		auto response = cpr::Get(cpr::Url{ url.str() });

		if (response.status_code != 200 && response.status_code != 201)
			throw std::exception(std::format("[Sender] Invalid room ID < {} >\n", roomID).c_str());
		std::cout << std::format("[Sender] Connected to room < {} >\n", roomID);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return false;
	}
}