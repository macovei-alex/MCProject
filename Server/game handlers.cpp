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
			{literals::jsonKeys::settings::drawTime, m_games[gameID].GetGameSettings().m_drawTime},
			{literals::jsonKeys::settings::roundCount, m_games[gameID].GetGameSettings().m_roundCount},
			{literals::jsonKeys::settings::chooseWordOptionCount, m_games[gameID].GetGameSettings().m_chooseWordOptionCount} };
			});


	CROW_ROUTE(m_app, literals::routes::game::settings::param).methods(crow::HTTPMethod::Put)
		([this](const crow::request& request, uint64_t gameID) {

		if (request.body.empty())
		{
			Log("Empty request body", Logger::Level::Error);
			return crow::response{ 404, "Empty request body" };
		}

		auto gameIt{ m_games.find(gameID) };

		if (gameIt == m_games.end())
		{
			std::string errorString{ std::format("Invalid game ID < {} >", gameID) };
			Log(errorString, Logger::Level::Error);
			return crow::response{ 404, errorString };
		}

		Game& game{ gameIt->second };
		auto jsonMap{ std::move(utils::ParseRequestBody(request.body)) };

		try
		{
			common::game::GameSettings gameSettings{
				static_cast<uint16_t>(std::stoi(jsonMap.find(literals::jsonKeys::settings::drawTime)->second)),
				static_cast<uint16_t>(std::stoi(jsonMap.find(literals::jsonKeys::settings::roundCount)->second)),
				static_cast<uint16_t>(std::stoi(jsonMap.find(literals::jsonKeys::settings::chooseWordOptionCount)->second)) };

			game.SetGameSettings(gameSettings);
		}
		catch (...)
		{
			Log("Invalid parameter values", Logger::Level::Error);
			return crow::response{ 404, "Invalid parameter values" };
		}

		return crow::response{ 200 };
			});


	CROW_ROUTE(m_app, literals::routes::game::state::param).methods(crow::HTTPMethod::Get)
		([this](const crow::request& request, uint64_t roomID) {

		auto gameIt{ m_games.find(roomID) };
		if (gameIt == m_games.end())
		{
			Log(std::format("Invalid room ID < {} >", roomID), Logger::Level::Error);
			return m_errorValue;
		}

		Game& game{ gameIt->second };
		auto gameState{ game.GetGameState() };

		if (gameState == common::game::GameState::DRAW_AND_GUESS)
			return crow::json::wvalue{
					{literals::jsonKeys::game::state, static_cast<uint64_t>(gameState)},
					{literals::jsonKeys::game::timeRemaining, static_cast<uint64_t>(
						game.GetGameSettings().m_drawTime) - game.GetTurn().GetTimer().count()} };

		return crow::json::wvalue{
				{literals::jsonKeys::game::state, static_cast<uint64_t>(gameState)},
				{literals::jsonKeys::game::timeRemaining, 0} };
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
				.GetPlayer(request.url_params.get(literals::jsonKeys::account::username))
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

		gameIt->second.GetPlayersMutex().lock();
		for (const auto& player : gameIt->second.GetPlayers())
		{
			playerScoresJson.emplace_back(crow::json::wvalue{
				{literals::jsonKeys::account::username, player.GetName()},
				{literals::jsonKeys::game::score, player.GetScore()} });
		}
		gameIt->second.GetPlayersMutex().unlock();

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

		Game& game{ gameIt->second };

		if (game.GetGameState() != common::game::GameState::PICK_WORD)
		{
			Log("Turn is already started", Logger::Level::Error);
			return m_errorValue;
		}

		crow::json::wvalue::list wordsJson;
		auto words{ std::move(m_database->GetRandomWords(game.GetGameSettings().m_chooseWordOptionCount)) };

		for (auto& word : words)
			wordsJson.emplace_back(crow::json::wvalue{ {literals::jsonKeys::game::word, std::move(word)} });

		return crow::json::wvalue{ wordsJson };
			});


	CROW_ROUTE(m_app, literals::routes::game::words::param).methods(crow::HTTPMethod::Put)
		([this](const crow::request& request, uint64_t roomID) {

		auto gameIt{ m_games.find(roomID) };
		if (gameIt == m_games.end())
		{
			std::string retStr{ std::move(std::format("Invalid room ID < {} >", roomID)) };
			Log(retStr, Logger::Level::Error);
			return crow::response{ 404, retStr };
		}

		Game& game{ gameIt->second };

		if (game.GetGameState() != common::game::GameState::PICK_WORD)
		{
			std::string retStr{ "Turn is already started" };
			Log(retStr, Logger::Level::Error);
			return crow::response{ 404, retStr };
		}

		if (request.body.empty())
		{
			std::string retStr{ "Empty request body" };
			Log(retStr, Logger::Level::Error);
			return crow::response{ 404, retStr };
		}

		auto jsonMap{ std::move(utils::ParseRequestBody(request.body)) };
		auto wordIt{ jsonMap.find(literals::jsonKeys::game::word) };

		if (wordIt == jsonMap.end())
		{
			std::string retStr{ "Invalid request body" };
			Log(retStr, Logger::Level::Error);
			return crow::response{ 404, retStr };
		}

		try
		{
			game.SetTurnWord(wordIt->second);
		}
		catch (const std::exception& exception)
		{
			std::string retStr{ std::move(std::format("Invalid word < {} >", wordIt->second)) };
			Log(retStr, Logger::Level::Error);
			return crow::response{ 404, retStr };
		}

		return crow::response{ 200, "Word set successful" };
			});


	CROW_ROUTE(m_app, literals::routes::game::start::param).methods(crow::HTTPMethod::Put)
		([this](const crow::request& request, uint64_t roomID) {

		auto gameIt{ m_games.find(roomID) };
		if (gameIt == m_games.end())
		{
			std::string retStr{ std::move(std::format("Invalid room ID < {} >", roomID)) };
			Log(retStr, Logger::Level::Error);
			return crow::response{ 404, retStr };
		}

		if (gameIt->second.GetGameState() != common::game::GameState::NONE)
		{
			std::string retStr{ "Game is already started" };
			Log(retStr, Logger::Level::Error);
			return crow::response{ 404, retStr };
		}

		/*if (gameIt->second.GetPlayers().size() < 2)
		{
			std::string retStr{ "Not enough players" };
			Log(retStr, Logger::Level::Error);
			return crow::response{ 404, retStr };
		}*/

		Game& game{ gameIt->second };
		std::thread gameThread{ [&game]() { game.Run(); } };
		gameThread.detach();

		return crow::response{ 200, "Game started" };
			});


	return *this;
}