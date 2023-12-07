#include "services.h"
#include "..\Common\constantLiterals.h"

#include <iostream>
#include <crow.h>

uint64_t services::CreateRoom()
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::room::create;

		auto response = cpr::Get(cpr::Url{ url.str() });

		if (response.status_code != 200 && response.status_code != 201)
		{
			if (!response.reason.empty())
				std::cout << std::format("[Create] {}\n", response.reason);
			else
				std::cout << "[Create] Server didn't return an error explanation\n";
		}

		uint64_t roomID = crow::json::load(response.text)[literals::jsonKeys::room::ID].u();
		std::cout << std::format("[Create] New room with roomID < {} > created\n", roomID);
		return roomID;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
		return LONG_MAX;
	}
}

bool services::ConnectToRoom(uint64_t roomID)
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::room::connect << '/' << roomID;

		auto response = cpr::Get(cpr::Url{ url.str() });

		if (response.status_code != 200 && response.status_code != 201)
			throw std::exception(std::format("[Connect] Invalid room ID < {} >\n", roomID).c_str());
		std::cout << std::format("[Connect] Connected to room < {} >\n", roomID);
		return true;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return false;
	}
}

bool services::SignIn(const std::string& username, const std::string& password)
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::account::signIn;

		auto response = cpr::Get(
			cpr::Url{ url.str() },
			cpr::Parameters{
				{literals::jsonKeys::account::username, username},
				{literals::jsonKeys::account::password, password}
			});

		if (response.status_code != 200 && response.status_code != 201)
			throw std::exception(std::format("[Sign In] {}\n", response.text).c_str());
		std::cout << std::format("[Sign In] {}\n", response.text);
		return true;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return false;
	}
}

bool services::SignUp(const std::string& username, const std::string& password)
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::account::singUp;

		auto response = cpr::Post(
			cpr::Url{ url.str() },
			cpr::Payload{
				{literals::jsonKeys::account::username, username},
				{literals::jsonKeys::account::password, password}
			});

		if (response.status_code != 200 && response.status_code != 201)
		{
			if (!response.reason.empty())
				throw std::exception(std::format("[Sign Up] {}\n", response.reason).c_str());
			else
				throw std::exception("[Sign Up] Server didn't return an error explanation\n");
		}

		std::cout << std::format("[Sign Up] Account created with username < {} >\n", username);
		std::cout << std::format("[Sign Up] Signed up as < {} >\n", username);
		return true;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return false;
	}
}

bool services::SignOut(const std::string& username)
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::account::signOut;

		auto response = cpr::Put(
			cpr::Url{ url.str() },
			cpr::Payload{
				{literals::jsonKeys::account::username, username}
			});

		if (response.status_code != 200 && response.status_code != 201)
		{
			if (!response.reason.empty())
				throw std::exception(std::format("[Sender] {}\n", response.reason).c_str());
			else
				throw std::exception("[Sender] Server didn't return an error explanation\n");
		}

		std::cout << "[Sender] Succesfully signed out\n";
		return true;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return false;
	}
}

void services::SendNewMessage(std::ostream& outputStream, const std::string& username, const std::string& content, uint64_t gameID)
{
	static const std::string urlBlueprint{ std::string{literals::routes::baseAddress} + std::string{literals::routes::game::chat::simple} + "/"};

	std::string url{ urlBlueprint + std::to_string(gameID) };

	try 
	{
		auto response = cpr::Put(
			cpr::Url{ url },
			cpr::Payload{
				{literals::jsonKeys::message::author, username},
				{literals::jsonKeys::message::content, content} });

		if (response.status_code != 200 && response.status_code != 201)
		{
			if (!response.reason.empty())
				std::cout << std::format("[Message sender] {}\n", response.reason);
			else
				std::cout << "[Message sender] Server didn't provide an explanation\n";
		}
	}
	catch (const std::exception& ex)
	{
		outputStream << ex.what();
	}
}

void services::ReceiveNewMessages(std::ostream& outputStream, const std::string& username, uint64_t gameID)
{
	static const std::string urlBlueprint = { std::string{literals::routes::baseAddress} + std::string{literals::routes::game::chat::simple} + "/" };

	static uint64_t lastTimestamp = 0;
	static bool serverErrorDetected = false;
	
	std::string url{ urlBlueprint + std::to_string(gameID) };

	try
	{
		auto response = cpr::Get(
			cpr::Url{ url },
			cpr::Parameters{
				{literals::jsonKeys::message::author, username},
				{literals::jsonKeys::message::timestamp, std::to_string(lastTimestamp)}
			});

		if (response.status_code != 200 && response.status_code != 201)
		{
			if (!serverErrorDetected)
			{
				if (!response.reason.empty())
					outputStream << std::format("[Message receiver] {}\n", response.reason);
				else
					outputStream << "[Message receiver] Server didn't provide an explanation\n";
				serverErrorDetected = true;
			}
		}
		else
			serverErrorDetected = false;

		if (serverErrorDetected)
			return;

		auto messagesJsonList = crow::json::load(response.text);
		if (messagesJsonList.size() == 0)
			lastTimestamp = max(lastTimestamp, 1);
		else if (messagesJsonList.size() == 1 && messagesJsonList[0].has(literals::error))
		{
			outputStream << "[Message receiver]: Json has error";
			return;
		}
		else
			lastTimestamp = messagesJsonList[messagesJsonList.size() - 1][literals::jsonKeys::message::timestamp].u() + 1;

		for (auto& messageJson : messagesJsonList)
		{
			uint64_t messageTimestampMillis = messageJson[literals::jsonKeys::message::timestamp].u();
			auto dateTime = utils::DateTimeFromInteger(messageTimestampMillis);

			outputStream << std::format("[{} at {}]: {}\n",
				std::string{ std::move(messageJson[literals::jsonKeys::message::author].s()) },
				dateTime,
				std::string{ std::move(messageJson[literals::jsonKeys::message::content].s()) });
		}
	}
	catch (const std::exception& exception)
	{
		outputStream << "[Message receiver]: " << exception.what();
	}
}

void services::ReceiveImageUpdates(std::ostream& outputStream, uint64_t gameID)
{
	static const std::string urlBlueprint = { std::string{literals::routes::baseAddress} + std::string{literals::routes::game::draw::updates} + "/" };

	static uint64_t lastTimestamp = 0;
	static bool serverErrorDetected = false;

	std::string url{ urlBlueprint + std::to_string(gameID) };

	try
	{
		auto response = cpr::Get(
			cpr::Url{ url },
			cpr::Parameters{ {literals::jsonKeys::draw::timestamp, std::to_string(lastTimestamp)} });

		if (response.status_code != 200 && response.status_code != 201)
		{
			if (!serverErrorDetected)
			{
				if (!response.reason.empty())
					outputStream << std::format("[Drawing updater] {}\n", response.reason);
				else
					outputStream << "[Drawing updater] Server didn't provide an explanation\n";
				serverErrorDetected = true;
			}
		}
		else
			serverErrorDetected = false;

		if (serverErrorDetected)
			return;

		auto pointsJsonList = crow::json::load(response.text);
		if (pointsJsonList.size() == 0)
			lastTimestamp = max(lastTimestamp, 1);
		else if (pointsJsonList.size() == 1 && pointsJsonList[0].has(literals::error))
		{
			outputStream << ("[Drawing updater] Json has error");
			return;
		}
		else
			lastTimestamp = pointsJsonList[pointsJsonList.size() - 1][literals::jsonKeys::draw::timestamp].u() + 1;

		for (auto& pointJson : pointsJsonList)
		{
			int32_t rgbColor = pointJson[literals::jsonKeys::draw::color].i();
			uint8_t r = (rgbColor >> 16) & 0xFF;
			uint8_t g = (rgbColor >> 8) & 0xFF;
			uint8_t b = rgbColor & 0xFF;

			outputStream << std::format("[Drawing updater]: New point({}, {}, color({}, {}, {})) received\n",
				pointJson[literals::jsonKeys::draw::pointX].i(),
				pointJson[literals::jsonKeys::draw::pointY].i(),
				r, g, b);
		}
	}
	catch (const std::exception& exception)
	{
		outputStream << "[Drawer]: " << exception.what();
	}
}

void services::MessageSender(uint64_t gameID, const std::string& username, bool* keepGoing)
{
	while (*keepGoing)
	{
		/*try
		{
			std::string message;
			std::getline(std::cin, message);
			services::SendNewMessage(std::cout, username, message, gameID);
		}
		catch (const std::exception& e)
		{
			std::cout << e.what();
		}*/

		std::string message;
		std::getline(std::cin, message);
		services::SendNewMessage(std::cout, username, message, gameID);
	}
}

void services::MessagesReceiver(uint64_t gameID, const std::string& username, bool* keepGoing)
{
	using namespace std::literals::chrono_literals;

	while (*keepGoing)
	{
		/*try
		{
			services::ReceiveNewMessages(std::cout, gameID);
		}
		catch (const std::exception& exception)
		{
			std::cout << "[Listener] Error detected: " << exception.what() << "\nThe app will close after you enter any character\n";
			*keepGoing = false;
		}*/

		services::ReceiveNewMessages(std::cout, username, gameID);
		std::this_thread::sleep_for(0.5s);
	}
}

void services::ImageUpdatesReceiver(uint64_t gameID, bool* keepGoing)
{
	using namespace std::literals::chrono_literals;

	while (*keepGoing)
	{
		/*try
		{
			services::SingleImageLoadHandler(std::cout, gameID);
		}
		catch (const std::exception& exception)
		{
			std::cout << "[Listener] Error detected: " << exception.what() << "\nThe app will close after you enter any character\n";
			*keepGoing = false;
		}*/

		services::ReceiveImageUpdates(std::cout, gameID);
		std::this_thread::sleep_for(0.5s);
	}
}
