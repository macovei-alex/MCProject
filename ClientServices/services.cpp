#include "services.h"

#include <iostream>
#include <format>
#include <crow.h>

#include "constantLiterals.h"
#include "..\UtilsDLL\utilsDLL.h"

uint64_t services::CreateRoom(const std::string& username, std::ostream& outStream, std::ostream& errStream)
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

		if (!services::ConnectToRoom(roomID, username))
			throw std::exception("Could not connect to the newly created room");

		errStream << std::format("[Create] New room with roomID < {} > created\n", roomID);
		return roomID;
	}
	catch (const std::exception& exception)
	{
		errStream << std::format("[Create room]: {}\n", exception.what());
		return LONG_MAX;
	}
}

bool services::ConnectToRoom(uint64_t roomID, const std::string& username, std::ostream& outStream, std::ostream& errStream)
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::game::connect::simple << '/' << roomID;

		auto response = cpr::Get(
			cpr::Url{ url.str() },
			cpr::Parameters{ {literals::jsonKeys::account::username, username} });

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
			cpr::Payload{ {literals::jsonKeys::account::username, username} });

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
				{literals::jsonKeys::message::text, content} });

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

std::vector<common::Message> services::ReceiveNewMessages(const std::string& username, uint64_t gameID, std::ostream& outStream, std::ostream& errStream)
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

		std::vector<common::Message> messages;

		for (auto& messageJson : messagesJsonList)
		{
			common::Message message{
				std::string{ std::move(messageJson[literals::jsonKeys::message::author].s()) },
				std::string{ std::move(messageJson[literals::jsonKeys::message::text].s()) },
				messageJson[literals::jsonKeys::message::timestamp].u() };

			messages.emplace_back(std::move(message));
		}

		return messages;
	}
	catch (const std::exception& exception)
	{
		errStream << "[Message receiver]: " << exception.what() << '\n';
		return {};
	}
}

void services::SendImageUpdates(uint64_t gameID, const std::vector<common::img::Point>& points, std::ostream& outStream, std::ostream& errStream)
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

std::vector<common::img::Point> services::ReceiveImageUpdates(uint64_t gameID, std::ostream& errStream)
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
				serverErrorDetected = true;
				if (!response.reason.empty())
					throw std::exception(response.reason.c_str());
				else
					throw std::exception("Server didn't provide an explanation");
			}
		}
		else
			serverErrorDetected = false;

		if (serverErrorDetected)
			return {};

		auto pointsJsonList = crow::json::load(response.text);
		if (pointsJsonList.size() == 0)
			lastTimestamp = max(lastTimestamp, 1);
		else if (pointsJsonList.size() == 1 && pointsJsonList[0].has(literals::error))
			throw std::exception("Json has error");
		else
			lastTimestamp = pointsJsonList[pointsJsonList.size() - 1][literals::jsonKeys::draw::timestamp].u() + 1;

		std::vector<common::img::Point> points;
		points.reserve(pointsJsonList.size());

		for (auto& pointJson : pointsJsonList)
		{
			common::img::Point point{
				pointJson[literals::jsonKeys::draw::pointX].i(),
				pointJson[literals::jsonKeys::draw::pointY].i(),
				pointJson[literals::jsonKeys::draw::color].i() };

			points.emplace_back(point.x, point.y, common::img::Color{ point.color.r, point.color.g, point.color.b });
		}

		return points;
	}
	catch (const std::exception& exception)
	{
		errStream << "[Drawing updater]: " << exception.what() << '\n';
		throw exception;
	}
}

void services::SendGameSettings(uint64_t gameID, const common::game::GameSettings& gameSettings, std::ostream& outStream, std::ostream& errStream)
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

common::game::GameSettings services::ReceiveGameSettings(uint64_t gameID, std::ostream& errStream)
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
		errStream << "[Settings receiver]: " << execption.what() << '\n';
	}
}

std::pair<common::game::GameState, uint64_t> services::ReceiveGameStateAndTime(uint64_t gameID, std::ostream& errStream)
{
	static const std::string urlBlueprint = { std::string{literals::routes::baseAddress} + std::string{literals::routes::game::state::simple} + "/" };

	static bool serverErrorDetected = false;

	std::string url{ urlBlueprint + std::to_string(gameID) };

	try
	{
		auto response = cpr::Get(cpr::Url{ url });

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

		auto responseJson{ std::move(crow::json::load(response.text)) };
		if (responseJson.has(literals::error))
			throw std::exception("Json has error");

		return std::make_pair(
			static_cast<common::game::GameState>(responseJson[literals::jsonKeys::game::state].u()),
			responseJson[literals::jsonKeys::game::timeRemaining].u());
	}
	catch (const std::exception& exception)
	{
		errStream << "[Game state updater]: " << exception.what() << '\n';
		return { common::game::GameState::NONE, 0 };
	}
}

common::game::PlayerRole services::ReceivePlayerRole(uint64_t roomID, const std::string& username, std::ostream& errStream)
{
	static const std::string urlBlueprint{ std::string{literals::routes::baseAddress} + std::string{literals::routes::game::playerRole::simple} + "/" };

	std::string url{ urlBlueprint + std::to_string(roomID) };

	try
	{
		auto response = cpr::Get(
			cpr::Url{ url },
			cpr::Parameters{ {literals::jsonKeys::account::username, username} });

		if (response.status_code != 200 && response.status_code != 201)
			throw std::exception(response.reason.c_str());


		auto responseJson{ std::move(crow::json::load(response.text)) };
		if (responseJson.has(literals::error))
			throw std::exception("Json has error");

		return static_cast<common::game::PlayerRole>(responseJson[literals::jsonKeys::player::role].u());
	}
	catch (const std::exception& exception)
	{
		errStream << "[Player role receiver]: " << exception.what() << '\n';
		return common::game::PlayerRole::NONE;
	}
}

std::vector<std::pair<std::string, int32_t>> services::ReceivePlayerScores(uint64_t gameID, std::ostream& errStream)
{
	static const std::string urlBlueprint{ std::string{literals::routes::baseAddress} + std::string{literals::routes::game::scores::simple} + "/" };

	std::string url{ urlBlueprint + std::to_string(gameID) };

	try
	{
		auto response = cpr::Get(cpr::Url{ url });

		auto responseJson{ std::move(crow::json::load(response.text)) };
		if (responseJson.has(literals::error))
			throw std::exception("Json has error");

		std::vector<std::pair<std::string, int32_t>> scores;
		for (auto& json : responseJson)
		{
			std::string name{ std::move(json[literals::jsonKeys::account::username].s()) };
			int32_t score{ static_cast<int32_t>(json[literals::jsonKeys::game::score].i()) };

			scores.emplace_back(std::move(name), score);
		}

		return scores;
	}
	catch (const std::exception& exception)
	{
		errStream << exception.what() << '\n';
	}
}
