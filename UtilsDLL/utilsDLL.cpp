#include "utilsDLL.h"

#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

std::string utils::GetHashSHA256(const std::string& input)
{
	std::string hash;
	CryptoPP::SHA256 sha256;
	CryptoPP::StringSource(input, true, new CryptoPP::HashFilter(sha256, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hash))));
	return hash;
}

std::vector<std::string> utils::SplitToVector(const std::string& str, const std::string& delim)
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
	if (found == std::string::npos)
		return { str, "" };
	std::string first{ str.substr(0, found) };
	std::string second{ str.substr(found + 1, str.size() - found - 1) };
	return { first, second };
}

uint64_t utils::MillisFromDateTime(std::chrono::system_clock::time_point dateTime)
{
	return std::chrono::duration_cast
		<std::chrono::milliseconds>
		(dateTime.time_since_epoch()).count();
}

std::chrono::system_clock::time_point utils::DateTimeFromMillis(uint64_t millis)
{
	return std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>
		(std::chrono ::duration_cast<std::chrono::seconds>
			(std::chrono::milliseconds{ millis }));
}
