#pragma once

#include <chrono>

namespace chr = std::chrono;

namespace utils
{
	enum class Menu1Options: uint16_t
	{
		SIGN_IN = 1,
		SIGN_UP,
		EXIT_1
	};

	enum class Menu2Options : uint16_t
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
	void MessageSender(uint64_t gameID, const std::string& username, bool* keepGoing);
	void MessagesReceiver(uint64_t gameID, const std::string& username, bool* keepGoing);
	void ImageUpdatesReceiver(uint64_t gameID, bool* keepGoing);
	void GameStateReceiver(uint64_t gameID, bool* keepGoing);
}