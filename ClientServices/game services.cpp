#include "services.h"

void services::SendGameSettings(uint64_t gameID, const common::game::GameSettings& gameSettings, std::ostream& outStream, std::ostream& errStream)
{
	static const std::string urlBlueprint = { std::string{literals::routes::baseAddress} + std::string{literals::routes::game::settings::simple} + "/" };

	try
	{
		std::string url{ urlBlueprint + std::to_string(gameID) };

		auto response = cpr::Put(
			cpr::Url{ url },
			cpr::Payload{
				{literals::jsonKeys::settings::drawTime, std::to_string(gameSettings.m_drawTime)},
				{literals::jsonKeys::settings::roundCount, std::to_string(gameSettings.m_roundCount)},
				{literals::jsonKeys::settings::chooseWordOptionCount, std::to_string(gameSettings.m_chooseWordOptionCount)},
			});

		if (response.status_code != 200 && response.status_code != 201)
		{
			if (!response.reason.empty())
				throw std::exception{ response.reason.c_str() };
			else
				throw std::exception{ "Server didn't provide an explanation" };
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
			throw std::exception{ std::format("Invalid game ID < {} >", gameID).c_str() };
	}
	catch (const std::exception& execption)
	{
		errStream << "[Settings receiver]: " << execption.what() << '\n';
	}
}

std::pair<common::game::GameState, uint64_t> services::ReceiveGameStateAndTime(uint64_t gameID, std::ostream& errStream)
{
	static const std::string urlBlueprint{ std::string{literals::routes::baseAddress} + std::string{literals::routes::game::state::simple} + "/" };
	static bool serverErrorDetected{ false };

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
					throw std::exception{ response.reason.c_str() };
				else
					throw std::exception{ "Server didn't provide an explanation" };
			}
		}
		else
			serverErrorDetected = false;

		auto responseJson{ std::move(crow::json::load(response.text)) };
		if (responseJson.has(literals::error))
			throw std::exception{ "Json has error" };

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
			throw std::exception{ response.reason.c_str() };


		auto responseJson{ std::move(crow::json::load(response.text)) };
		if (responseJson.has(literals::error))
			throw std::exception{ "Json has error" };

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
			throw std::exception{ "Json has error" };

		std::vector<std::pair<std::string, int32_t>> scores;

		std::ranges::for_each(responseJson, [&scores](const auto& playerScore) {
			std::string name{ std::move(playerScore[literals::jsonKeys::account::username].s()) };
			int32_t score{ static_cast<int32_t>(playerScore[literals::jsonKeys::game::score].i()) };

			scores.emplace_back(std::move(name), score);
			});

		/*for (auto& playerScore : responseJson)
		{
			std::string name{ std::move(playerScore[literals::jsonKeys::account::username].s()) };
			int32_t score{ static_cast<int32_t>(playerScore[literals::jsonKeys::game::score].i()) };

			scores.emplace_back(std::move(name), score);
		}*/

		return scores;
	}
	catch (const std::exception& exception)
	{
		errStream << exception.what() << '\n';
	}
}

std::vector<std::string> services::ReceiveWordOptions(uint64_t roomID, std::ostream& errStream)
{
	static const std::string urlBlueprint{ std::string{literals::routes::baseAddress} + std::string{literals::routes::game::words::simple} + "/" };

	std::string url{ urlBlueprint + std::to_string(roomID) };

	try
	{
		auto response = cpr::Get(cpr::Url{ url });

		auto responseJson{ std::move(crow::json::load(response.text)) };
		if (responseJson.has(literals::error))
			throw std::exception{ "Json has error" };

		std::vector<std::string> words;

		std::ranges::for_each(responseJson, [&words](const auto& wordJson) {
			std::string word{ std::move(wordJson[literals::jsonKeys::game::word].s()) };
			words.emplace_back(std::move(word));
			});

		/*for (auto& wordJson : responseJson)
		{
			std::string word{ std::move(wordJson[literals::jsonKeys::game::word].s()) };
			words.emplace_back(std::move(word));
		}*/

		return words;
	}
	catch (const std::exception& exception)
	{
		errStream << exception.what() << '\n';
		throw exception;
	}
}

void services::SendGuessingWord(uint64_t roomID, const std::string& word, std::ostream& errStream)
{
	static const std::string urlBlueprint{ std::string{literals::routes::baseAddress} + literals::routes::game::words::simple + "/" };

	std::string url{ urlBlueprint + std::to_string(roomID) };

	try
	{
		auto response = cpr::Put(
			cpr::Url{ url },
			cpr::Payload{ {literals::jsonKeys::game::word, word} });

		if (response.status_code != 200 && response.status_code != 201)
			throw std::exception{ response.reason.c_str() };
	}
	catch (const std::exception& exception)
	{
		errStream << exception.what() << '\n';
	}
}

void services::StartGame(uint64_t roomID, std::ostream& errStream)
{
	static const std::string urlBlueprint{ std::string{literals::routes::baseAddress} + std::string{literals::routes::game::start::simple} + "/" };

	std::string url{ urlBlueprint + std::to_string(roomID) };

	try
	{
		auto response = cpr::Put(cpr::Url{ url });
	}
	catch (const std::exception& exception)
	{
		errStream << exception.what() << '\n';
	}
}

