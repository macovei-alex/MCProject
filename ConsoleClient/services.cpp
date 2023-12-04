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
				std::cout << std::format("[Sender] {}\n", response.reason);
			else
				std::cout << "[Sender] Server didn't return an error explanation\n";
		}

		uint64_t roomID = crow::json::load(response.text)[literals::jsonKeys::room::ID].u();
		std::cout << std::format("[Sender] New room with roomID < {} > created\n", roomID);
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
				throw std::exception(std::format("[Sender] {}\n", response.reason).c_str());
			else
				throw std::exception("[Sender] Server didn't return an error explanation\n");
		}

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

void services::SingleChatLoadHandler(std::ostream& outputStream, cpr::Response& response, uint64_t& lastTimeMillis)
{
	static bool serverErrorDetected = false;
	try
	{
		if (response.status_code != 200 && response.status_code != 201)
		{
			if (!serverErrorDetected)
			{
				if (!response.reason.empty())
					outputStream << std::format("[Listener] {}\n", response.reason);
				else
					outputStream << "[Listener] Server didn't provide an explanation\n";
				serverErrorDetected = true;
			}
			return;
		}
		else
			serverErrorDetected = false;

		auto messagesJsonList = crow::json::load(response.text);
		if (messagesJsonList.size() == 0)
			lastTimeMillis = max(lastTimeMillis, 1);
		else if (messagesJsonList.size() == 1 && messagesJsonList[0].has(literals::error))
			throw std::exception("[Listener] Error: communication error");
		else
			lastTimeMillis = messagesJsonList[messagesJsonList.size() - 1][literals::jsonKeys::message::timestamp].u() + 1;

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
	catch (std::exception exception)
	{
		throw exception;
	}
}

void services::SingleImageLoadHandler(std::ostream& outputStream, cpr::Response& response, uint64_t& lastTimeMillis)
{
	static bool serverErrorDetected = false;
	try
	{
		if (response.status_code != 200 && response.status_code != 201)
		{
			if (!serverErrorDetected)
			{
				if (!response.reason.empty())
					outputStream << std::format("[Listener] {}\n", response.reason);
				else
					outputStream << "[Listener] Server didn't provide an explanation\n";
				serverErrorDetected = true;
			}
			return;
		}
		else
			serverErrorDetected = false;

		auto pointsJsonList = crow::json::load(response.text);
		if (pointsJsonList.size() == 0)
			lastTimeMillis = max(lastTimeMillis, 1);
		else if (pointsJsonList.size() == 1 && pointsJsonList[0].has(literals::error))
			throw std::exception("[Listener] Error: communication error");
		else
			lastTimeMillis = pointsJsonList[pointsJsonList.size() - 1][literals::jsonKeys::draw::timestamp].u() + 1;

		for (auto& pointJson : pointsJsonList)
		{
			int32_t rgbColor = pointJson[literals::jsonKeys::draw::color].i();
			uint8_t r = (rgbColor >> 16) & 0xFF;
			uint8_t g = (rgbColor >> 8) & 0xFF;
			uint8_t b = rgbColor & 0xFF;

			outputStream << std::format("[Drawer]: New point({}, {}, color({}, {}, {})) received\n",
				pointJson[literals::jsonKeys::draw::pointX].i(),
				pointJson[literals::jsonKeys::draw::pointY].i(),
				r, g, b);
		}
	}
	catch (std::exception exception)
	{
		throw exception;
	}
}

void services::MessagesSender(uint64_t gameID, const std::string& username, bool* keepGoing)
{
	std::stringstream url;
	url << literals::routes::baseAddress << literals::routes::game::chat::simple << '/' << gameID;

	while (*keepGoing)
	{
		try
		{
			std::string message;
			std::getline(std::cin, message);
			auto response = cpr::Put(
				cpr::Url{ url.str() },
				cpr::Payload{
					{literals::jsonKeys::message::author, username},
					{literals::jsonKeys::message::content, message} }
			);
			if (response.status_code != 200 && response.status_code != 201)
			{
				if (!response.reason.empty())
					std::cout << std::format("[Sender] {}\n", response.reason);
				else
					std::cout << "[Sender] Server didn't provide an explanation\n";
			}
		}
		catch (const std::exception& e)
		{
			*keepGoing = false;
			std::cout << e.what();
		}
	}
}

void services::MessagesReceiver(uint64_t gameID, const std::string& username, bool* keepGoing)
{
	using namespace std::literals::chrono_literals;

	std::stringstream url;
	url << literals::routes::baseAddress << literals::routes::game::chat::param << '/' << gameID;
	std::cout << std::format("[Listener] Listening to {}\n", url.str());

	uint64_t lastTimeMillis = 0;

	while (*keepGoing)
	{
		try
		{
			auto response = cpr::Get(
				cpr::Url{ url.str() },
				cpr::Parameters{
					{literals::jsonKeys::message::author, username},
					{literals::jsonKeys::message::timestamp, std::to_string(lastTimeMillis)}
				});
			services::SingleChatLoadHandler(std::cout, response, lastTimeMillis);
		}
		catch (std::exception exception)
		{
			std::cout << "[Listener] Error detected: " << exception.what() << "\nThe app will close after you enter any character\n";
			*keepGoing = false;
		}
		std::this_thread::sleep_for(0.5s);
	}
}

void services::ImageUpdatesReceiver(uint64_t gameID, bool* keepGoing)
{
	using namespace std::literals::chrono_literals;

	std::stringstream url;
	url << literals::routes::baseAddress << literals::routes::game::draw::getUpdates << '/' << gameID;
	std::cout << std::format("[Listener] Listening to {}\n", url.str());

	uint64_t lastTimeMillis = 0;

	while (*keepGoing)
	{
		try
		{
			auto response = cpr::Get(
				cpr::Url{ url.str() },
				cpr::Parameters{
					{literals::jsonKeys::draw::timestamp, std::to_string(lastTimeMillis)}
				});
			services::SingleImageLoadHandler(std::cout, response, lastTimeMillis);
		}
		catch (std::exception exception)
		{
			std::cout << "[Listener] Error detected: " << exception.what() << "\nThe app will close after you enter any character\n";
			*keepGoing = false;
		}
		std::this_thread::sleep_for(0.5s);
	}
}
