#pragma once

#include <chrono>

namespace chr = std::chrono;

namespace utils
{
	enum Menu1Options : uint16_t
	{
		SIGN_IN = 1,
		SIGN_UP,
		EXIT_1
	};

	enum Menu2Options : uint16_t
	{
		CREATE_ROOM = 1,
		JOIN_ROOM,
		SIGN_OUT,
		EXIT_2
	};

	uint64_t GetInt(const char* message = nullptr);
	std::string GetString(const char* message = nullptr);
	void PrintMenu1();
	void PrintMenu2();
}