#include "clientConnectionHandlers.h"
#include "..\Common\constantLiterals.h"

#include <iostream>
#include <crow.h>

uint64_t handlers::CreateRoom()
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

bool handlers::ConnectToRoom(uint64_t roomID)
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

bool handlers::SignIn(const std::string& username, const std::string& password)
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

bool handlers::SignUp(const std::string& username, const std::string& password)
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

bool handlers::SignOut(const std::string& username)
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::sign::out;

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

void handlers::SingleChatLoadHandler(std::ostream& outputStream, cpr::Response& response, uint64_t& lastTimeMillis)
{
	using namespace std::literals::chrono_literals;

	static bool serverErrorDetected = false;
	static const uint64_t assumedReceivingDelayMillis = 250;
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

			std::this_thread::sleep_for(0.5s);
			return;
		}
		else
			serverErrorDetected = false;

		auto messagesJson = crow::json::load(response.text);
		if (messagesJson.size() != 0)
			lastTimeMillis = messagesJson[messagesJson.size() - 1][literals::jsonKeys::message::timePoint].u() + 1;
		else
			lastTimeMillis = utils::NowAsInteger() - assumedReceivingDelayMillis;

		for (auto& messageJson : messagesJson)
		{
			uint64_t messageTimePointMillis = messageJson[literals::jsonKeys::message::timePoint].u();
			auto dateTime = utils::DateTimeFromInteger(messageTimePointMillis);

			outputStream << std::format("[{} at {}]: {}\n",
				std::string{ std::move(messageJson[literals::jsonKeys::message::author].s()) },
				dateTime,
				std::string{ std::move(messageJson[literals::jsonKeys::message::content].s()) });
		}

		std::this_thread::sleep_for(0.5s);
	}
	catch (std::exception exception)
	{
		throw exception;
	}
}

void handlers::Sender(uint64_t gameID, const std::string& username, bool* keepGoing)
{
	std::stringstream url;
	url << literals::routes::baseAddress << literals::routes::game::chat << '/' << gameID;

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

void handlers::Receiver(uint64_t gameID, const std::string& username, bool* keepGoing)
{
	using namespace std::literals::chrono_literals;

	std::stringstream url;
	url << literals::routes::baseAddress << literals::routes::game::chat << '/' << gameID;
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
					{literals::jsonKeys::message::timePoint, std::to_string(lastTimeMillis)}
				});
			handlers::SingleChatLoadHandler(std::cout, response, lastTimeMillis);
		}
		catch (std::exception exception)
		{
			std::cout << "[Listener] Error detected: " << exception.what() << "\nThe app will close after you enter any character\n";
			*keepGoing = false;
		}
	}
}
