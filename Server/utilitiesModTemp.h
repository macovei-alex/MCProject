#pragma once

#include <vector>
#include <string>
#include <chrono>

namespace utils
{
	struct Message
	{
		std::string content;
		std::string author;
		uint64_t timeMilliseconds;

		Message() = default;
		Message(std::string&& content, std::string&& author, uint64_t milliseconds);
	};

	enum Lobby
	{
		player_join,
		player_left,
		game_begin
	};

	std::vector<std::string> SplitToVec(const std::string& str, const std::string& delim);
	std::pair<std::string, std::string> SplitToPair(const std::string& str, const std::string& delim);
	std::string DecodeMessage(const std::string& message);
	uint64_t DateTimeAsInteger(std::chrono::system_clock::time_point dateTime);
}