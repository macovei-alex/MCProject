#include "inputHandling.h"

#include <iostream>

uint64_t GetInt()
{
	uint64_t input;
	std::cin >> input;
	return input;
}

std::string&& GetString()
{
	std::string input;
	std::getline(std::cin, input);
	return std::move(input);
}

void PrintMenu1()
{
	std::cout << "Choose an option:\n"
		<< "1. Create a new room\n"
		<< "2. Join an existing room\n"
		<< "3. Exit\n";
}