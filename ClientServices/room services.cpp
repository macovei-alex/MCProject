#include "services.h"

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