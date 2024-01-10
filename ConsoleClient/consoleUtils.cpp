#include "consoleUtils.h"

#include <iostream>
#include <format>

#include "services.h"

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
		<< '\t' << static_cast<uint16_t>(utils::Menu1Options::SIGN_IN) << ". Sign into an existing account\n"
		<< '\t' << static_cast<uint16_t>(utils::Menu1Options::SIGN_UP) << ". Create a new account\n"
		<< '\t' << static_cast<uint16_t>(utils::Menu1Options::EXIT_1) << ". Exit\n";
	std::cout << "******************************************\n\n";
}

void utils::PrintMenu2()
{
	std::cout << "\n******************************************\n";
	std::cout << "Options:\n"
		<< '\t' << static_cast<uint16_t>(utils::Menu2Options::CREATE_ROOM) << ". Create a new room\n"
		<< '\t' << static_cast<uint16_t>(utils::Menu2Options::JOIN_ROOM) << ". Join an existing room\n"
		<< '\t' << static_cast<uint16_t>(utils::Menu2Options::SIGN_OUT) << ". Sign out\n"
		<< '\t' << static_cast<uint16_t>(utils::Menu2Options::EXIT_2) << ". Exit\n";
	std::cout << "******************************************\n\n";
}

void utils::MessageSender(uint64_t gameID, const std::string& username, bool* keepGoing)
{
	while (*keepGoing)
	{
		std::string message;
		std::getline(std::cin, message);
		services::SendNewMessage(username, message, gameID);
	}
}

void utils::MessagesReceiver(uint64_t gameID, const std::string& username, bool* keepGoing)
{
	using namespace std::literals::chrono_literals;

	while (*keepGoing)
	{
		auto messages{ std::move(services::ReceiveNewMessages(username, gameID)) };
		for (const auto& message : messages)
		{
			std::cout << std::format("({}) [{}]: {}\n", utils::DateTimeFromMillis(message.timestamp), message.author, message.text);
		}
		std::this_thread::sleep_for(0.5s);
	}
}

void utils::ImageUpdatesReceiver(uint64_t gameID, bool* keepGoing)
{
	using namespace std::literals::chrono_literals;

	while (*keepGoing)
	{
		auto points{ services::ReceiveImageUpdates(gameID) };
		for(const auto& point : points)
			std::cout << std::format("({},{},{})\n", point.x, point.y, point.color.ToInt32());

		std::this_thread::sleep_for(0.5s);
	}
}

void utils::GameStateReceiver(uint64_t gameID, bool* keepGoing)
{
	using namespace std::literals::chrono_literals;

	while (*keepGoing)
	{
		auto gameSettingsPair{ services::ReceiveGameStateAndTime(gameID) };
		if (gameSettingsPair.first != common::game::GameState::NONE)
			std::cout << static_cast<uint16_t>(gameSettingsPair.first) << ', ' << gameSettingsPair.second << '\n';

		std::this_thread::sleep_for(0.5s);
	}
}
