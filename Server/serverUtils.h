#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <windows.h>

namespace utils
{
	std::string GetHash(const std::string& input);
	void NavigateToProjectDirectory();
}