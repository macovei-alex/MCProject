#pragma once

#include <chrono>

namespace chr = std::chrono;

namespace utils
{
	uint8_t GetInt();
	std::string&& GetString();
	void PrintMenu1();
	chr::time_point<chr::system_clock, chr::seconds> DateTimeFromInteger(uint64_t millis);
	uint64_t NowAsInteger();

	uint64_t CreateRoom();
	bool ConnectToRoom(uint64_t roomID);
}