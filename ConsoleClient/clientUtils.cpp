#include "clientUtils.h"

#include <iostream>
#include <format>

uint64_t utils::GetInt(const char* message)
{
	if (message)
		std::cout << message;
	uint64_t input;
	std::cin >> input;
	while (std::cin.rdbuf()->in_avail() != 0)
		std::cin.get();
	return input;
}

std::string utils::GetString(const char* message)
{
	if (message)
		std::cout << message;
	std::string input;
	std::getline(std::cin, input);
	while (std::cin.rdbuf()->in_avail() != 0)
		std::cin.get();
	return input;
}

void utils::PrintMenu1()
{
	std::cout << "\n******************************************\n";
	std::cout << "Options:\n"
		<< '\t' << utils::Menu1Options::SIGN_IN << ". Sign into an existing account\n"
		<< '\t' << utils::Menu1Options::SIGN_UP << ". Create a new account\n"
		<< '\t' << utils::Menu1Options::EXIT_1 << ". Exit\n";
	std::cout << "******************************************\n\n";
}

void utils::PrintMenu2()
{
	std::cout << "\n******************************************\n";
	std::cout << "Options:\n"
		<< '\t' << utils::Menu2Options::CREATE_ROOM << ". Create a new room\n"
		<< '\t' << utils::Menu2Options::JOIN_ROOM << ". Join an existing room\n"
		<< '\t' << utils::Menu2Options::SIGN_OUT << ". Sign out\n"
		<< '\t' << utils::Menu2Options::EXIT_2 << ". Exit\n";
	std::cout << "******************************************\n\n";
}

chr::time_point<chr::system_clock, chr::seconds> utils::DateTimeFromInteger(uint64_t millis)
{
	return chr::time_point<chr::system_clock, chr::seconds>
		(chr::duration_cast<chr::seconds>
			(chr::milliseconds{ millis }));
}

uint64_t utils::NowAsInteger()
{
	return chr::duration_cast<chr::milliseconds>
		(chr::system_clock::now().time_since_epoch()).count();
}