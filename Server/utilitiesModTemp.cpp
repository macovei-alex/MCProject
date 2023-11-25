#include "utilitiesModTemp.h";

utils::Message::Message(std::string&& content, std::string&& author, uint64_t milliseconds) :
	content{ std::move(content) },
	author{ std::move(author) },
	timeMilliseconds{ milliseconds }
{
	/* EMPTY */
}

std::vector<std::string> utils::SplitToVec(const std::string& str, const std::string& delim)
{
	std::vector<std::string> result;
	size_t startIndex = 0;

	for (size_t found = str.find(delim); found != std::string::npos; found = str.find(delim, startIndex))
	{
		result.emplace_back(str.begin() + startIndex, str.begin() + found);
		startIndex = found + delim.size();
	}
	if (startIndex != str.size())
		result.emplace_back(str.begin() + startIndex, str.end());
	return result;
}

std::pair<std::string, std::string> utils::SplitToPair(const std::string& str, const std::string& delim)
{
	size_t found = str.find(delim);
	std::string first = str.substr(0, found);
	std::string second = str.substr(found + 1, str.size() - found - 1);
	return { first, second };
}

std::string utils::DecodeMessage(const std::string& message)
{
	std::string decodedMessage = "";
	for (int i = 0; i < message.length(); i++)
	{
		if (message[i] == '%')
		{
			std::string hex = message.substr(i + 1, 2);
			int decimal = std::stoi(hex, nullptr, 16);
			decodedMessage += static_cast<char>(decimal);
			i += 2;
		}
		else
		{
			decodedMessage += message[i];
		}
	}
	return decodedMessage;
}

uint64_t utils::DateTimeAsInteger(std::chrono::system_clock::time_point dateTime)
{
	return std::chrono::duration_cast
		<std::chrono::milliseconds>
		(dateTime.time_since_epoch()).count();
}