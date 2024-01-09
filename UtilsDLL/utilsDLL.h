#pragma once

#include <string>
#include <vector>
#include <chrono>

namespace utils
{
	__declspec(dllexport) int add(int a, int b);
	__declspec(dllexport) std::string GetHashSHA256(const std::string& input);
	__declspec(dllexport) std::vector<std::string> SplitToVector(const std::string& str, const std::string& delim);
	__declspec(dllexport) std::pair<std::string, std::string> SplitToPair(const std::string& str, const std::string& delim);
	__declspec(dllexport) uint64_t DateTimeAsInteger(std::chrono::system_clock::time_point dateTime = std::chrono::system_clock::now());
}