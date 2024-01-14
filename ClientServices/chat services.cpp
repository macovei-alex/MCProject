#include "services.h"

bool services::SendNewMessage(const std::string& username, const std::string& content, uint64_t gameID, std::ostream& outStream, std::ostream& errStream)
{
	static const std::string urlBlueprint{ std::string{literals::routes::baseAddress} + literals::routes::game::chat::simple + "/" };

	std::string url{ urlBlueprint + std::to_string(gameID) };

	try
	{
		auto response{ cpr::Put(
			cpr::Url{ url },
			cpr::Payload{
				{literals::jsonKeys::message::author, username},
				{literals::jsonKeys::message::text, content} }) };

		if (response.status_code == literals::correctGuessResponse)
			return true;

		else if (response.status_code == literals::incorrectGuessResponse)
			return false;

		else
		{
			if (!response.reason.empty())
				throw std::exception{ response.reason.c_str() };
			else
				throw std::exception{ "Server didn't provide an explanation" };
		}
	}
	catch (const std::exception& exception)
	{
		errStream << "[Message sender]: " << exception.what() << '\n';
		return false;
	}
}

std::vector<common::Message> services::ReceiveNewMessages(const std::string& username, uint64_t gameID, std::ostream& outStream, std::ostream& errStream)
{
	static const std::string urlBlueprint{ std::string{literals::routes::baseAddress} + literals::routes::game::chat::simple + "/" };
	static uint64_t lastTimestamp{ 0 };
	static bool serverErrorDetected{ false };

	std::string url{ urlBlueprint + std::to_string(gameID) };

	try
	{
		auto response{ cpr::Get(
			cpr::Url{ url },
			cpr::Parameters{
				{literals::jsonKeys::message::author, username},
				{literals::jsonKeys::message::timestamp, std::to_string(lastTimestamp)}
			}) };

		if (response.status_code != 200 && response.status_code != 201)
		{
			if (!serverErrorDetected)
			{
				serverErrorDetected = true;
				if (!response.reason.empty())
					throw std::exception{ response.reason.c_str() };
				else
					throw std::exception{ "Server didn't provide an explanation" };
			}
		}
		else
			serverErrorDetected = false;

		auto messagesJsonList{ crow::json::load(response.text) };

		if (messagesJsonList.size() == 0)
			lastTimestamp = std::max(lastTimestamp, 1ULL);

		else if (messagesJsonList.size() == 1 && messagesJsonList[0].has(literals::error))
			throw std::exception{ "Json has error" };

		else
			lastTimestamp = messagesJsonList[messagesJsonList.size() - 1][literals::jsonKeys::message::timestamp].u() + 1;

		std::vector<common::Message> messages;

		std::ranges::for_each(messagesJsonList, [&messages](const auto& messageJson) {
			messages.emplace_back(common::Message{
				std::string{ std::move(messageJson[literals::jsonKeys::message::text].s()) },
				std::string{ std::move(messageJson[literals::jsonKeys::message::author].s()) },
				messageJson[literals::jsonKeys::message::timestamp].u() });
		});

		return messages;
	}
	catch (const std::exception& exception)
	{
		errStream << "[Message receiver]: " << exception.what() << '\n';
		return {};
	}
}