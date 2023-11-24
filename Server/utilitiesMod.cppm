export module utilitiesMod;

import <vector>;
import <string>;

namespace utils 
{
	export struct Message
	{
		std::string content;
		std::string author;
		time_t timestamp;

		Message() = default;
		Message(std::string&& content, std::string&& author, time_t timestamp);
	};

	export enum Lobby
	{
		player_join,
		player_left,
		game_begin
	};

	export std::vector<std::string> splitToVec(const std::string& str, const std::string& delim);
	export std::pair<std::string, std::string> splitToPair(const std::string& str, const std::string& delim);
	export std::string decodeMessage(const std::string& message);
}