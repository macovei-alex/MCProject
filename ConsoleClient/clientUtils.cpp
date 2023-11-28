#include "clientUtils.h"

#include <iostream>
#include <format>
#include <cpr/cpr.h>
#include <crow.h>

#include "..\Common\constantLiterals.h"

uint64_t utils::GetInt(const char* message)
{
	if (message)
		std::cout << message;
	uint64_t input;
	std::cin >> input;
	if (std::cin.rdbuf()->in_avail() != 0)
		std::cin.get();
	return input;
}

std::string utils::GetString(const char* message)
{
	if (message)
		std::cout << message;
	std::string input;
	std::getline(std::cin, input);
	if (std::cin.rdbuf()->in_avail() != 0)
		std::cin.get();
	return input;
}

void utils::PrintMenu1()
{
	std::cout << "\n******************************************\n";
	std::cout << "Options:\n"
		<< "\t1. Sign into an existing account\n"
		<< "\t2. Create a new account\n"
		<< "\t3. Exit\n";
	std::cout << "******************************************\n\n";
}

void utils::PrintMenu2()
{
	std::cout << "\n******************************************\n";
	std::cout << "Options:\n"
		<< "\t1. Create a new room\n"
		<< "\t2. Join an existing room\n"
		<< "\t3. Sign out\n"
		<< "\t4. Exit\n";
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

uint64_t utils::CreateRoom()
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::room::create;

		auto response = cpr::Get(cpr::Url{ url.str() });

		if (response.status_code != 200 && response.status_code != 201)
			std::cout << std::format("[Sender] {}\n", response.reason);

		uint64_t roomID = crow::json::load(response.text)[literals::jsonKeys::room::roomID].u();
		std::cout << std::format("[Sender] New room with roomID < {} > created\n", roomID);
		return roomID;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
		return LONG_MAX;
	}
}

bool utils::ConnectToRoom(uint64_t roomID)
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::room::connect << '/' << roomID;

		auto response = cpr::Get(cpr::Url{ url.str() });

		if (response.status_code != 200 && response.status_code != 201)
			throw std::exception(std::format("[Sender] Invalid room ID < {} >\n", roomID).c_str());
		std::cout << std::format("[Sender] Connected to room < {} >\n", roomID);
		return true;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return false;
	}
}

bool utils::SignIn(const std::string& username, const std::string& password)
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::sign::in;

		auto response = cpr::Get(
			cpr::Url{ url.str() },
			cpr::Parameters{
				{literals::jsonKeys::account::username, username},
				{literals::jsonKeys::account::password, password}
			});

		if (response.status_code != 200 && response.status_code != 201)
			throw std::exception(std::format("[Sender] {}\n", response.text).c_str());
		std::cout << std::format("[Sender] {}\n", response.text);
		return true;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return false;
	}
}

bool utils::SignUp(const std::string& username, const std::string& password)
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::sign::up;

		auto response = cpr::Post(
			cpr::Url{ url.str() },
			cpr::Payload{
				{literals::jsonKeys::account::username, username},
				{literals::jsonKeys::account::password, password}
			});

		if (response.status_code != 200 && response.status_code != 201)
			throw std::exception(std::format("[Sender] {}\n", response.reason).c_str());

		std::cout << std::format("[Sender] Account created with username < {} >\n", username);
		std::cout << std::format("[Sender] Signed up as < {} >\n", username);
		return true;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return false;
	}
}