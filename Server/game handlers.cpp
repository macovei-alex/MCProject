#include "Server.h"

Server& Server::GameHandlers()
{
	CROW_ROUTE(m_app, literals::routes::game::settings::param).methods(crow::HTTPMethod::Get)
		([this](const crow::request& request, uint64_t gameID) {

		if (m_games.find(gameID) == m_games.end())
		{
			Log(std::format("Invalid game ID < {} >", gameID), Logger::Level::Error);
			return m_errorValue;
		}

		Log(std::format("Game settings requested for game < {} >", gameID), Logger::Level::Info);
		return crow::json::wvalue{
			{literals::jsonKeys::settings::drawTime, m_games[gameID].GetGameSettings().GetDrawTime()},
			{literals::jsonKeys::settings::roundCount, m_games[gameID].GetGameSettings().GetRoundCount()},
			{literals::jsonKeys::settings::chooseWordOptionCount, m_games[gameID].GetGameSettings().GetChooseWordOptionCount()} };
			});


	CROW_ROUTE(m_app, literals::routes::game::settings::param).methods(crow::HTTPMethod::Put)
		([this](const crow::request& request, uint64_t gameID) {

		if (request.body.empty())
		{
			Log("Empty request body", Logger::Level::Error);
			return crow::response(404, "Empty request body");
		}

		if (m_games.find(gameID) == m_games.end())
		{
			std::string errorString{ std::format("Invalid game ID < {} >", gameID) };
			Log(errorString, Logger::Level::Error);
			return crow::response(404, errorString);
		}

		auto& game = m_games[gameID];

		auto jsonMap{ utils::ParseRequestBody(request.body) };

		try
		{
			uint64_t drawTime = std::stoi(jsonMap.find(literals::jsonKeys::settings::drawTime)->second);
			game.GetGameSettings().SetDrawTime(drawTime);

			int64_t roundCount = std::stoi(jsonMap.find(literals::jsonKeys::settings::roundCount)->second);
			game.GetGameSettings().SetRoundCount(roundCount);

			int64_t chooseWordOptionCount = std::stoi(jsonMap.find(literals::jsonKeys::settings::chooseWordOptionCount)->second);
			game.GetGameSettings().SetDrawTime(chooseWordOptionCount);
		}
		catch (const std::exception& exception)
		{
			Log("Invalid parameter values", Logger::Level::Error);
			return crow::response(404, "Invalid parameter values");
		}

		return crow::response(200);
			});


	CROW_ROUTE(m_app, literals::routes::game::state::param).methods(crow::HTTPMethod::Get)
		([this](const crow::request& request, uint64_t roomID) {

		/*if (request.url_params.keys().empty())
		{
			Log("Empty request body", Logger::Level::Error);
			return errorValue;
		}*/

		auto gameIt{ m_games.find(roomID) };
		if (gameIt == m_games.end())
		{
			Log(std::format("Invalid room ID < {} >", roomID), Logger::Level::Error);
			return m_errorValue;
		}

		auto gameState{ gameIt->second.GetGameState() };

		if (gameState == common::game::GameState::NONE)
			return crow::json::wvalue{
				{literals::jsonKeys::game::state, static_cast<uint64_t>(gameIt->second.GetGameState())},
				{literals::jsonKeys::game::timeRemaining, 0} };

		return crow::json::wvalue{
				{literals::jsonKeys::game::state, static_cast<uint64_t>(gameIt->second.GetGameState())},
				{literals::jsonKeys::game::timeRemaining, gameIt->second.GetTurn().GetPlayTime()} };
			});


	CROW_ROUTE(m_app, literals::routes::game::playerRole::param).methods(crow::HTTPMethod::Get)
		([this](const crow::request& request, uint64_t roomID) {

		auto gameIt{ m_games.find(roomID) };
		if (gameIt == m_games.end())
		{
			Log(std::format("Invalid room ID < {} >", roomID), Logger::Level::Error);
			return m_errorValue;
		}

		try
		{
			auto playerRole{ gameIt->second
				.GetPlayer(
					request.url_params.get(literals::jsonKeys::account::username))
				.GetRole() };

			return crow::json::wvalue{ {literals::jsonKeys::player::role, static_cast<uint64_t>(playerRole)} };
		}
		catch (const std::exception& exception)
		{
			Log(exception.what(), Logger::Level::Error);
			return m_errorValue;
		}

		return m_errorValue;

			});


	CROW_ROUTE(m_app, literals::routes::game::scores::param).methods(crow::HTTPMethod::Get)
		([this](const crow::request& request, uint64_t roomID) {

		auto gameIt{ m_games.find(roomID) };
		if (gameIt == m_games.end())
		{
			Log(std::format("Invalid room ID < {} >", roomID), Logger::Level::Error);
			return m_errorValue;
		}

		crow::json::wvalue::list playerScoresJson;
		for (auto& player : gameIt->second.GetPlayers())
		{
			playerScoresJson.emplace_back(crow::json::wvalue{
				{literals::jsonKeys::account::username, player.GetName()},
				{literals::jsonKeys::game::score, player.GetScore()} });
		}

		return crow::json::wvalue{ playerScoresJson };
			});


	CROW_ROUTE(m_app, literals::routes::game::words::param).methods(crow::HTTPMethod::Get)
		([this](const crow::request& request, uint64_t roomID) {

		auto gameIt{ m_games.find(roomID) };
		if (gameIt == m_games.end())
		{
			Log(std::format("Invalid room ID < {} >", roomID), Logger::Level::Error);
			return m_errorValue;
		}

		crow::json::wvalue::list wordsJson;
		auto words{ std::move(m_database->GetRandomWords(gameIt->second.GetGameSettings().GetChooseWordOptionCount())) };

		for (auto& word : words)
			wordsJson.emplace_back(crow::json::wvalue{ {literals::jsonKeys::word, std::move(word)} });

		return crow::json::wvalue{ wordsJson };
			});


	CROW_ROUTE(m_app, literals::routes::game::words::param).methods(crow::HTTPMethod::Put)
		([this](const crow::request& request, uint64_t roomID) {

		auto gameIt{ m_games.find(roomID) };
		if (gameIt == m_games.end())
		{
			auto retStr{ std::move(std::format("Invalid room ID < {} >", roomID)) };
			Log(retStr, Logger::Level::Error);
			return crow::response(404, retStr);
		}

		if (request.body.empty())
		{
			auto retStr{ std::move(std::format("Empty request body")) };
			Log(retStr, Logger::Level::Error);
			return crow::response(404, retStr);
		}

		auto jsonMap{ std::move(utils::ParseRequestBody(request.body)) };
		auto wordIt{ jsonMap.find(literals::jsonKeys::word) };
		
		if ( wordIt == jsonMap.end())
		{
			auto retStr{ "Invalid request body" };
			Log(retStr, Logger::Level::Error);
			return crow::response(404, retStr);
		}

		return crow::response(200, "Word set successful");
			});

	return *this;
}