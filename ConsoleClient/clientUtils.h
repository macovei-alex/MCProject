#pragma once

#include <chrono>

namespace chr = std::chrono;

namespace utils
{
	uint64_t GetInt(const char* message = nullptr);
	std::string GetString(const char* message = nullptr);
	void PrintMenu1();
	void PrintMenu2();
	chr::time_point<chr::system_clock, chr::seconds> DateTimeFromInteger(uint64_t millis);
	uint64_t NowAsInteger();

	uint64_t CreateRoom();
	bool ConnectToRoom(uint64_t roomID);
	bool SignIn(const std::string& username, const std::string& password);
	bool SignUp(const std::string& username, const std::string& password);
}