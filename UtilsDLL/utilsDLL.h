#pragma once

#include <string>
#include <vector>
#include <chrono>

namespace utils
{
	__declspec(dllexport) std::string GetHashSHA256(const std::string& input);
	__declspec(dllexport) std::vector<std::string> SplitToVector(const std::string& str, const std::string& delim);
	__declspec(dllexport) std::pair<std::string, std::string> SplitToPair(const std::string& str, const std::string& delim);
	__declspec(dllexport) uint64_t MillisFromDateTime(std::chrono::system_clock::time_point dateTime = std::chrono::system_clock::now());
	__declspec(dllexport) std::chrono::system_clock::time_point DateTimeFromMillis(uint64_t millis);
}