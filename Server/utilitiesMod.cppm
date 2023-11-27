export module utilitiesMod;

import <vector>;
import <string>;
import <chrono>;

namespace utils
{
	export struct Message
	{
		std::string content;
		std::string author;
		uint64_t timeMilliseconds;

		Message() = default;
		Message(std::string&& content, std::string&& author, uint64_t milliseconds);
	};

	export std::vector<std::string> SplitToVec(const std::string& str, const std::string& delim);
	export std::pair<std::string, std::string> SplitToPair(const std::string& str, const std::string& delim);
	export std::string DecodeMessage(const std::string& message);
}