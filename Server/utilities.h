#pragma once
#include <string>
#include <Windows.h>

namespace utils 
{
	std::string getHash(const std::string& input);
	void NavigateToProjectDirectory();
}