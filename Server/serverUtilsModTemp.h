#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <variant>

namespace utils
{
	std::string DecodeMessage(const std::string& message);
	std::unordered_map<std::string, std::string> ParseRequestBody(const std::string& requestBody);
	std::vector<std::unordered_map<std::string, std::variant<int64_t, std::string>>> ListOfMapsFromJsonListStr(const std::string& jsonStr);
	void NavigateToProjectDirectory();
}