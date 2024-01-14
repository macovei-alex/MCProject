#include "services.h"

uint64_t services::CreateRoom(const std::string& username, std::ostream& outStream, std::ostream& errStream)
{
	static const std::string url{ std::string{literals::routes::baseAddress} + literals::routes::game::create };
	try
	{
		auto response{ cpr::Get(cpr::Url{ url }) };

		if (response.status_code != 200 && response.status_code != 201)
		{
			if (!response.reason.empty())
				throw std::exception{ response.reason.c_str() };
			else
				throw std::exception{ "Server didn't return an error explanation" };
		}

		uint64_t roomID{ crow::json::load(response.text)[literals::jsonKeys::game::ID].u() };

		if (!services::ConnectToRoom(roomID, username))
			throw std::exception{ "Could not connect to the newly created room" };

		errStream << std::format("[Create] New room with roomID < {} > created\n", roomID);
		return roomID;
	}
	catch (const std::exception& exception)
	{
		errStream << std::format("[Create room]: {}\n", exception.what());
		return MAXUINT64;
	}
}

bool services::ConnectToRoom(uint64_t roomID, const std::string& username, std::ostream& outStream, std::ostream& errStream)
{
	static const std::string urlBlueprint{ std::string{literals::routes::baseAddress} + literals::routes::game::connect::simple + "/" };
	try
	{
		std::string url{ urlBlueprint + std::to_string(roomID) };
		auto response{ cpr::Get(
			cpr::Url{ url },
			cpr::Parameters{ {literals::jsonKeys::account::username, username} }) };

		if (response.status_code != 200 && response.status_code != 201)
			throw std::exception{ std::format("Invalid room ID < {} >", roomID).c_str() };

		outStream << std::format("[Connect] Connected to room < {} >\n", roomID);
		return true;
	}
	catch (const std::exception& exception)
	{
		errStream << "[Connect to room]: " << exception.what() << '\n';
		return false;
	}
}