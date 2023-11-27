#include "clientUtils.h"

#include <iostream>
#include <format>
#include <cpr/cpr.h>
#include <crow.h>

#include "..\Common\constantLiterals.h"

chr::time_point<chr::system_clock, chr::seconds> DateTimeFromInteger(uint64_t millis)
{
	return std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>
		(std::chrono::duration_cast<std::chrono::seconds>
			(std::chrono::milliseconds{ millis }));
}

uint64_t CreateRoom()
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

bool ConnectToRoom(uint64_t roomID)
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::room::connect << '/' << roomID;

		auto response = cpr::Get(cpr::Url{ url.str() });

		if (response.status_code != 200 && response.status_code != 201)
			throw std::exception(std::format("[Sender] Invalid room ID < {} >\n", roomID).c_str());
		std::cout << std::format("[Sender] Connected to room < {} > created\n", roomID);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return false;
	}
}