#include "services.h"

#include "../Common/constantLiterals.h"
#include "../Common/commonUtils.h"

#include <iostream>
#include <crow.h>

uint64_t services::CreateRoom()
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::game::create;

		auto response = cpr::Get(cpr::Url{ url.str() });

		if (response.status_code != 200 && response.status_code != 201)
		{
			if (!response.reason.empty())
				throw std::exception(std::format("[Create] {}", response.reason).c_str());
			else
				throw std::exception("[Create] Server didn't return an error explanation");
		}

		uint64_t roomID = crow::json::load(response.text)[literals::jsonKeys::game::ID].u();
		std::cout << std::format("[Create] New room with roomID < {} > created\n", roomID);
		return roomID;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return LONG_MAX;
	}
}

bool services::ConnectToRoom(uint64_t roomID)
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::game::connect << '/' << roomID;

		auto response = cpr::Get(cpr::Url{ url.str() });

		if (response.status_code != 200 && response.status_code != 201)
			throw std::exception(std::format("[Connect] Invalid room ID < {} >", roomID).c_str());
		std::cout << std::format("[Connect] Connected to room < {} >\n", roomID);
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
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
			throw std::exception(std::format("[Sign In] {}", response.text).c_str());
		std::cout << std::format("[Sign In] {}\n", response.text);
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
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
				throw std::exception(std::format("[Sign Up] {}", response.reason).c_str());
			else
				throw std::exception("[Sign Up] Server didn't return an error explanation");
		}

		std::cout << std::format("[Sign Up] Account created with username < {} >\n", username);
		std::cout << std::format("[Sign Up] Signed up as < {} >\n", username);
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
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
				throw std::exception(std::format("[Sender] {}", response.reason).c_str());
			else
				throw std::exception("[Sender] Server didn't return an error explanation");
		}

		std::cout << "[Sender] Succesfully signed out\n";
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return false;
	}
}

void services::SendNewMessage(std::ostream& errStream, const std::string& username, const std::string& content, uint64_t gameID)
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
				throw std::exception(std::format("[Message sender] {}", response.reason).c_str());
			else
				throw std::exception("[Message sender] Server didn't provide an explanation");
		}
	}
	catch (const std::exception& ex)
	{
		errStream << ex.what() << '\n';
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
				serverErrorDetected = true;
				if (!response.reason.empty())
					throw std::exception(std::format("[Message receiver] {}", response.reason).c_str());
				else
					throw std::exception("[Message receiver] Server didn't provide an explanation");
			}
		}
		else
			serverErrorDetected = false;

		auto messagesJsonList = crow::json::load(response.text);
		if (messagesJsonList.size() == 0)
			lastTimestamp = max(lastTimestamp, 1);
		else if (messagesJsonList.size() == 1 && messagesJsonList[0].has(literals::error))
			throw std::exception("[Message receiver]: Json has error");
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
		outputStream << "[Message receiver]: " << exception.what() << '\n';
	}
}

void services::SendImageUpdates(std::ostream& errStream, uint64_t gameID, const std::vector<utils::img::Point>& points)
{
	static const std::string urlBlueprint = { std::string{literals::routes::baseAddress} + std::string{literals::routes::game::draw::updates} + "/" };

	std::string url{ urlBlueprint + std::to_string(gameID) };

	try
	{
		crow::json::wvalue::list pointsJsonList;
		pointsJsonList.reserve(points.size());

		for (auto& point : points)
			pointsJsonList.emplace_back(crow::json::wvalue{ {
				{literals::jsonKeys::draw::pointX, point.x},
				{literals::jsonKeys::draw::pointY, point.y},
				{literals::jsonKeys::draw::color, point.color.ToInt32() }} });

		auto str = crow::json::wvalue(pointsJsonList).dump();

		auto response = cpr::Put(
			cpr::Url{ url },
			cpr::Payload{ {literals::jsonKeys::draw::points, str} });

		if (response.status_code != 200 && response.status_code != 201)
		{
			if (!response.reason.empty())
				throw std::exception(std::format("[Drawing sender] {}", response.reason).c_str());
			else
				throw std::exception("[Drawing sender] Server didn't provide an explanation");
		}
	}
	catch (const std::exception& exception)
	{
		errStream << "[Drawing sender]: " << exception.what() << '\n';
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
					throw std::exception(std::format("[Drawing updater] {}", response.reason).c_str());
				else
					throw std::exception("[Drawing updater] Server didn't provide an explanation");
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
			throw std::exception("[Drawing updater] Json has error");
		else
			lastTimestamp = pointsJsonList[pointsJsonList.size() - 1][literals::jsonKeys::draw::timestamp].u() + 1;

		for (auto& pointJson : pointsJsonList)
		{
			utils::img::Point point{
				pointJson[literals::jsonKeys::draw::pointX].i(),
				pointJson[literals::jsonKeys::draw::pointY].i(),
				pointJson[literals::jsonKeys::draw::color].i() };

			outputStream << std::format("[Drawing updater]: New point({}, {}, color({}, {}, {})) received\n", point.x, point.y, point.color.r, point.color.g, point.color.b);
		}
	}
	catch (const std::exception& exception)
	{
		outputStream << "[Drawer]: " << exception.what() << '\n';
	}
}

void services::SendGameSettings(std::ostream& errStream, uint64_t gameID, const GameSettings& gameSettings)
{
	static const std::string urlBlueprint = { std::string{literals::routes::baseAddress} + std::string{literals::routes::game::settings::simple} + "/" };

	try
	{
		std::string url{ urlBlueprint + std::to_string(gameID) };

		std::vector<std::pair<std::string, std::string>> payload;
		payload.reserve(3);

		if (gameSettings.GetDrawTime() != 0)
			payload.emplace_back(literals::jsonKeys::settings::drawTime, std::to_string(gameSettings.GetDrawTime()));

		if (gameSettings.GetRoundCount() != 0)
			payload.emplace_back(literals::jsonKeys::settings::roundCount, std::to_string(gameSettings.GetRoundCount()));

		if (gameSettings.GetChooseWordOptionCount() != 0)
			payload.emplace_back(literals::jsonKeys::settings::chooseWordOptionCount, std::to_string(gameSettings.GetChooseWordOptionCount()));

		auto response = cpr::Put(
			cpr::Url{ url },
			cpr::Payload{ payload.begin(), payload.end() });

		if (response.status_code != 200 && response.status_code != 201)
		{
			if (!response.reason.empty())
				throw std::exception(std::format("[Settings sender] {}", response.reason).c_str());
			else
				throw std::exception("[Settings sender] Server didn't provide an explanation");
		}
	}
	catch (const std::exception& exception)
	{
		errStream << "[Settings sender]: " << exception.what() << '\n';
	}
}

std::tuple<uint16_t, uint16_t, uint16_t> services::ReceiveGameSettings(std::ostream& outputStream, uint64_t gameID)
{
	static const std::string urlBlueprint{ std::string{literals::routes::baseAddress} + std::string{literals::routes::game::settings::simple} + "/" };

	try
	{
		std::string url{ urlBlueprint + std::to_string(gameID) };

		auto response = cpr::Get(cpr::Url{ url });

		if (response.status_code != 200 && response.status_code != 201)
			throw std::exception(std::format("[Settings receiver] Invalid game ID < {} >", gameID).c_str());
	}
	catch (const std::exception& execption)
	{
		throw std::exception(std::format("[Settings receiver] {}", execption.what()).c_str());
	}
}

void services::MessageSender(uint64_t gameID, const std::string& username, bool* keepGoing)
{
	while (*keepGoing)
	{
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
		services::ReceiveNewMessages(std::cout, username, gameID);
		std::this_thread::sleep_for(0.5s);
	}
}

void services::ImageUpdatesReceiver(uint64_t gameID, bool* keepGoing)
{
	using namespace std::literals::chrono_literals;

	while (*keepGoing)
	{
		services::ReceiveImageUpdates(std::cout, gameID);
		std::this_thread::sleep_for(0.5s);
	}
}
