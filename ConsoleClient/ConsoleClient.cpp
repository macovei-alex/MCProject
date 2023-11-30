#include <iostream>
#include <string>
#include <thread>

#include "clientConnectionHandlers.h"
#include "clientUtils.h"
#include "..\Common\constantLiterals.h"

int main()
{
	const std::string returnCommand = "/b";

	std::string username;
	std::string password;
	std::string repeatPassword;

menu1:
	utils::PrintMenu1();
	utils::Menu1Options option1 = static_cast<utils::Menu1Options>(utils::GetInt("Your option: "));
	switch (option1)
	{

	case utils::Menu1Options::SIGN_IN:
		bool isSignInCorrect;
		do {
			username = utils::GetString(std::format("Enter your username (or \"{}\" to go back to the menu): ", returnCommand).c_str());
			if (username == returnCommand)
				goto menu1;
			password = utils::GetString(std::format("Enter your password (or \"{}\" to go back to the menu): ", returnCommand).c_str());
			if (password == returnCommand)
				goto menu1;
			isSignInCorrect = handlers::SignIn(username, password);
			if (!isSignInCorrect)
			{
				isSignInCorrect = false;
				std::cout << "Please try again.\n";
			}
		} while (!isSignInCorrect);
		break;

	case utils::Menu1Options::SIGN_UP:
		bool isSamePassord;
		do {
			isSamePassord = true;
			username = utils::GetString(std::format("Enter your username (or \"{}\" to go back to the menu): ", returnCommand).c_str());
			if (username == returnCommand)
				goto menu1;
			password = utils::GetString(std::format("Enter your password (or \"{}\" to go back to the menu): ", returnCommand).c_str());
			if (password == returnCommand)
				goto menu1;
			repeatPassword = utils::GetString(std::format("Repeat the password (or \"{}\" to go back to the menu): ", returnCommand).c_str());
			if (repeatPassword == returnCommand)
				goto menu1;
			if (password != repeatPassword)
			{
				std::cout << "Passwords do not match. Please try again\n";
				isSamePassord = false;
			}
			else
				isSamePassord = handlers::SignUp(username, password);
		} while (!isSamePassord);
		break;

	case utils::Menu1Options::EXIT_1:
		return 0;

	default:
		std::cout << "Invalid option. Please try again.\n";
		goto menu1;
	}

	char answer = 'y';

menu2:
	utils::PrintMenu2();
	utils::Menu2Options option2 = static_cast<utils::Menu2Options>(utils::GetInt("Your option: "));
	uint64_t roomID;
	switch (option2)
	{

	case utils::Menu2Options::CREATE_ROOM:
		do {
			roomID = handlers::CreateRoom();
			if (roomID == LONG_MAX)
			{
				std::cout << "Invalid room ID. Do you want to try again? [y/n]\n"
					<< "Your answer: ";
				std::cin >> answer;
			}
		} while (roomID == LONG_MAX && answer == 'y');
		if (answer == 'n')
			goto menu2;
		break;

	case utils::Menu2Options::JOIN_ROOM:
		bool isGoodConnection;
		do {
			std::cout << "Enter room ID: ";
			roomID = utils::GetInt();
			isGoodConnection = handlers::ConnectToRoom(roomID);
			if (!isGoodConnection)
			{
				std::cout << std::format("Do you want to try again? [y/n]\n", roomID)
					<< "Your answer: ";
				std::cin >> answer;
			}
		} while (!isGoodConnection && answer == 'y');
		if (answer == 'n')
			goto menu2;
		break;

	case utils::Menu2Options::SIGN_OUT:
		handlers::SignOut(username);
		goto menu1;

	case utils::Menu2Options::EXIT_2:
		return 0;

	default:
		std::cout << "Invalid option. Please try again.\n";
		goto menu2;
	}

	bool keepGoing = true;
	std::thread sender(handlers::Sender, roomID, username, &keepGoing);
	std::thread receiver(handlers::Receiver, roomID, username, &keepGoing);

	sender.join();
	receiver.join();

	return 0;
}