#include "services.h"

#include "../Common/constantLiterals.h"
#include "../Common/commonUtils.h"

#include <iostream>
#include <crow.h>

uint64_t services::CreateRoom(std::ostream& outStream, std::ostream& errStream)
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::game::create;

		auto response = cpr::Get(cpr::Url{ url.str() });

		if (response.status_code != 200 && response.status_code != 201)
		{
			if (!response.reason.empty())
				throw std::exception(response.reason.c_str());
			else
				throw std::exception("Server didn't return an error explanation");
		}

		uint64_t roomID = crow::json::load(response.text)[literals::jsonKeys::game::ID].u();
		outStream << std::format("[Create] New room with roomID < {} > created\n", roomID);
		return roomID;
	}
	catch (const std::exception& exception)
	{
		errStream << "[Create room]: " << exception.what() << '\n';
		return LONG_MAX;
	}
}

bool services::ConnectToRoom(uint64_t roomID, std::ostream& outStream, std::ostream& errStream)
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::game::connect << '/' << roomID;

		auto response = cpr::Get(cpr::Url{ url.str() });

		if (response.status_code != 200 && response.status_code != 201)
			throw std::exception(std::format("Invalid room ID < {} >", roomID).c_str());
		outStream << std::format("[Connect] Connected to room < {} >\n", roomID);
		return true;
	}
	catch (const std::exception& exception)
	{
		errStream << "[Connect to room]: " << exception.what() << '\n';
		return false;
	}
}

bool services::SignIn(const std::string& username, const std::string& password, std::ostream& outStream, std::ostream& errStream)
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
		{
			if (!response.reason.empty())
				throw std::exception(response.reason.c_str());
			else
				throw std::exception("Server didn't return an error explanation");
		}

		outStream << std::format("[Sign In] {}\n", response.text);
		return true;
	}
	catch (const std::exception& exception)
	{
		errStream << "[Sign In]: " << exception.what() << '\n';
		return false;
	}
}

bool services::SignUp(const std::string& username, const std::string& password, std::ostream& outStream, std::ostream& errStream)
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
				throw std::exception(response.reason.c_str());
			else
				throw std::exception("Server didn't return an error explanation");
		}

		outStream << std::format("[Sign Up]: Account created with username < {} >\n", username);
		outStream << std::format("[Sign Up]: Signed up as < {} >\n", username);
		return true;
	}
	catch (const std::exception& exception)
	{
		errStream << "[Sign Up]: " << exception.what() << '\n';
		return false;
	}
}

bool services::SignOut(const std::string& username, std::ostream& outStream, std::ostream& errStream)
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::account::signOut;

		auto response = cpr::Put(
			cpr::Url{ url.str() },
			cpr::Payload{{literals::jsonKeys::account::username, username}});

		if (response.status_code != 200 && response.status_code != 201)
		{
			if (!response.reason.empty())
				throw std::exception(response.reason.c_str());
			else
				throw std::exception("Server didn't return an error explanation");
		}

		outStream << std::format("Signed out from < {} >\n", username);
		return true;
	}
	catch (const std::exception& exception)
	{
		errStream << "[Sign Out]: " << exception.what() << '\n';
		return false;
	}
}

void services::SendNewMessage(const std::string& username, const std::string& content, uint64_t gameID, std::ostream& outStream, std::ostream& errStream)
{
	static const std::string urlBlueprint{ std::string{literals::routes::baseAddress} + std::string{literals::routes::game::chat::simple} + "/" };

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
				throw std::exception(response.reason.c_str());
			else
				throw std::exception("Server didn't provide an explanation");
		}
	}
	catch (const std::exception& exception)
	{
		errStream << "[Message sender]: " << exception.what() << '\n';
	}
}

void services::ReceiveNewMessages(const std::string& username, uint64_t gameID, std::ostream& outStream, std::ostream& errStream)
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
				serverErrorDetected = true;
				if (!response.reason.empty())
					throw std::exception(response.reason.c_str());
				else
					throw std::exception("Server didn't provide an explanation");
			}
		}
		else
			serverErrorDetected = false;

		auto messagesJsonList = crow::json::load(response.text);
		if (messagesJsonList.size() == 0)
			lastTimestamp = max(lastTimestamp, 1);
		else if (messagesJsonList.size() == 1 && messagesJsonList[0].has(literals::error))
			throw std::exception("Json has error");
		else
			lastTimestamp = messagesJsonList[messagesJsonList.size() - 1][literals::jsonKeys::message::timestamp].u() + 1;

		for (auto& messageJson : messagesJsonList)
		{
			uint64_t messageTimestampMillis = messageJson[literals::jsonKeys::message::timestamp].u();
			auto dateTime = utils::DateTimeFromInteger(messageTimestampMillis);

			outStream << std::format("[{} at {}]: {}\n",
				std::string{ std::move(messageJson[literals::jsonKeys::message::author].s()) },
				dateTime,
				std::string{ std::move(messageJson[literals::jsonKeys::message::content].s()) });
		}
	}
	catch (const std::exception& exception)
	{
		errStream << "[Message receiver]: " << exception.what() << '\n';
	}
}

void services::SendImageUpdates(uint64_t gameID, const std::vector<utils::img::Point>& points, std::ostream& outStream, std::ostream& errStream)
{
	static const std::string urlBlueprint = { std::string{literals::routes::baseAddress} + std::string{literals::routes::game::draw::updates} + "/" };

	std::string url{ urlBlueprint + std::to_string(gameID) };

	try
	{
		crow::json::wvalue test;
		crow::json::wvalue::list pointsJsonList;
		pointsJsonList.reserve(points.size());

		for (auto& point : points)
			pointsJsonList.emplace_back(crow::json::wvalue{ {
				{literals::jsonKeys::draw::pointX, point.x},
				{literals::jsonKeys::draw::pointY, point.y},
				{literals::jsonKeys::draw::color, point.color.ToInt32()}} });
		
		auto flattened = crow::json::wvalue(pointsJsonList);
		std::string str = flattened.dump();
		std::cout << str;

		auto response = cpr::Put(
			cpr::Url{ url },
			cpr::Payload{ {literals::jsonKeys::draw::points, str} });

		if (response.status_code != 200 && response.status_code != 201)
		{
			if (!response.reason.empty())
				throw std::exception(response.reason.c_str());
			else
				throw std::exception("Server didn't provide an explanation");
		}
	}
	catch (const std::exception& exception)
	{
		errStream << "[Drawing sender]: " << exception.what() << '\n';
	}
}

void services::ReceiveImageUpdates(uint64_t gameID, std::ostream& outStream, std::ostream& errStream)
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
					throw std::exception(response.reason.c_str());
				else
					throw std::exception("Server didn't provide an explanation");
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
			throw std::exception("Json has error");
		else
			lastTimestamp = pointsJsonList[pointsJsonList.size() - 1][literals::jsonKeys::draw::timestamp].u() + 1;

		for (auto& pointJson : pointsJsonList)
		{
			utils::img::Point point{
				pointJson[literals::jsonKeys::draw::pointX].i(),
				pointJson[literals::jsonKeys::draw::pointY].i(),
				pointJson[literals::jsonKeys::draw::color].i() };

			outStream << std::format("[Drawing updater]: New point({}, {}, color({}, {}, {})) received\n", point.x, point.y, point.color.r, point.color.g, point.color.b);
		}
	}
	catch (const std::exception& exception)
	{
		errStream << "[Drawing updater]: " << exception.what() << '\n';
	}
}

void services::SendGameSettings(uint64_t gameID, const GameSettings& gameSettings, std::ostream& outStream, std::ostream& errStream)
{
	static const std::string urlBlueprint = { std::string{literals::routes::baseAddress} + std::string{literals::routes::game::settings::simple} + "/" };

	try
	{
		std::string url{ urlBlueprint + std::to_string(gameID) };

		auto response = cpr::Put(
			cpr::Url{ url },
			cpr::Payload{
				{literals::jsonKeys::settings::drawTime, std::to_string(gameSettings.GetDrawTime())},
				{literals::jsonKeys::settings::roundCount, std::to_string(gameSettings.GetRoundCount())},
				{literals::jsonKeys::settings::chooseWordOptionCount, std::to_string(gameSettings.GetChooseWordOptionCount())},
			});

		if (response.status_code != 200 && response.status_code != 201)
		{
			if (!response.reason.empty())
				throw std::exception(response.reason.c_str());
			else
				throw std::exception("Server didn't provide an explanation");
		}
	}
	catch (const std::exception& exception)
	{
		errStream << "[Settings sender]: " << exception.what() << '\n';
	}
}

GameSettings services::ReceiveGameSettings(uint64_t gameID, std::ostream& outStream, std::ostream& errStream)
{
	static const std::string urlBlueprint{ std::string{literals::routes::baseAddress} + std::string{literals::routes::game::settings::simple} + "/" };

	try
	{
		std::string url{ urlBlueprint + std::to_string(gameID) };

		auto response = cpr::Get(cpr::Url{ url });

		if (response.status_code != 200 && response.status_code != 201)
			throw std::exception(std::format("Invalid game ID < {} >", gameID).c_str());
	}
	catch (const std::exception& execption)
	{
		outStream << "[Settings receiver]: " << execption.what() << '\n';
	}
}

void services::MessageSender(uint64_t gameID, const std::string& username, bool* keepGoing)
{
	while (*keepGoing)
	{
		std::string message;
		std::getline(std::cin, message);
		services::SendNewMessage(username, message, gameID);
	}
}

void services::MessagesReceiver(uint64_t gameID, const std::string& username, bool* keepGoing)
{
	using namespace std::literals::chrono_literals;

	while (*keepGoing)
	{
		services::ReceiveNewMessages(username, gameID);
		std::this_thread::sleep_for(0.5s);
	}
}

void services::ImageUpdatesReceiver(uint64_t gameID, bool* keepGoing)
{
	using namespace std::literals::chrono_literals;

	while (*keepGoing)
	{
		services::ReceiveImageUpdates(gameID);
		std::this_thread::sleep_for(0.5s);
	}
}
