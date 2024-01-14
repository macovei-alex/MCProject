#include "Game.h"

#include "constantLiterals.h"

Game::Game() noexcept :
	m_players{},
	m_roundNumber{ 0 },
	m_ownerID{ 0 },
	m_gameSettings{},
	m_gameState{ common::game::GameState::NONE },
	m_turn{},
	m_image{},
	m_chat{},
	m_stopped{ false }
{
	/* empty */
}

Game::Game(Game&& other) noexcept :
	m_players{ std::move(other.m_players) },
	m_roundNumber{ other.m_roundNumber },
	m_ownerID{ other.m_ownerID },
	m_gameSettings{ std::move(other.m_gameSettings) },
	m_turn{ std::move(other.m_turn) },
	m_image{ std::move(other.m_image) },
	m_chat{ std::move(other.m_chat) },
	m_gameState{ std::move(other.m_gameState.load()) },
	m_stopped{ other.m_stopped }
{
	other.m_roundNumber = 0;
	other.m_ownerID = 0;
	other.m_gameState = common::game::GameState::NONE;
}

Game& Game::operator=(Game&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}

	m_players = std::move(other.m_players);
	m_roundNumber = std::move(other.m_roundNumber);
	m_ownerID = std::move(other.m_ownerID);
	m_turn = std::move(other.m_turn);
	m_gameSettings = std::move(other.m_gameSettings);
	m_image = std::move(other.m_image);
	m_chat = std::move(other.m_chat);
	m_gameState = other.m_gameState.load();
	m_stopped = other.m_stopped;

	other.m_roundNumber = 0;
	other.m_ownerID = 0;
	other.m_gameState = common::game::GameState::NONE;
	other.m_stopped = true;

	return *this;
}

const std::vector<Player>& Game::GetPlayers()
{
	return std::as_const(m_players.GetRef());
}

const Player& Game::GetPlayer(const std::string& name)
{
	std::lock_guard<std::mutex> lock{ m_players.GetMutex() };
	if (const auto playerFoundIt{ std::ranges::find_if(m_players.GetRef(),
				[&name](const Player& player) -> bool {
			return player.GetName() == name;
		}) };
		playerFoundIt != m_players.GetRef().end())
	{
		return *playerFoundIt;
	}

	throw std::exception{ "Player not found" };
}

uint8_t Game::GetRoundNumber() const
{
	return m_roundNumber;
}

common::game::GameSettings Game::GetGameSettings()
{
	return m_gameSettings;
}

void Game::SetGameSettings(common::game::GameSettings gameSettings)
{
	m_gameSettings = gameSettings;
}

const Turn& Game::GetTurn() const
{
	return m_turn;
}

common::game::GameState Game::GetGameState() const
{
	return m_gameState.load();
}

void Game::SetGameState(common::game::GameState gameState)
{
	m_gameState = gameState;
}

std::mutex& Game::GetPlayersMutex()
{
	return m_players.GetMutex();
}

void Game::SetTurnWord(std::string&&)
{
	m_turn.SetWord(std::move(m_turn.GetWord()));
}

void Game::SetTurnWord(const std::string& word)
{
	m_turn.SetWord(word);
}

crow::json::wvalue Game::GetUpdatesJsonAfter(uint64_t timestamp)
{
	std::lock_guard<std::mutex> lock{ m_image.GetMutex() };
	return Image::UpdatesToJson(m_image.GetRef().GetUpdatesAfter(timestamp));
}

void Game::ChatEmplace(common::Message&& message)
{
	m_chat.GetMutex().lock();
	m_chat.GetRef().Emplace(std::move(message));
	m_chat.GetMutex().unlock();
}

bool Game::IsRunning() const
{
	return !m_stopped;
}

void Game::Run()
{
	m_stopped = false;
	Reset();

	std::unordered_map<std::string, bool> playersDone{ m_players.GetRef().size() };

	const auto& findNextPlayerLambda{ [&playersDone](std::vector<Player>& m_players) {
			return std::ranges::find_if(m_players,
				[&playersDone](const Player& player) {
					if (playersDone.find(player.GetName()) == playersDone.end())
					{
						playersDone.emplace(player.GetName(), false);
						return true;
					}
					return !playersDone.at(player.GetName()); });
			}
	};

	while (!m_stopped && m_roundNumber < m_gameSettings.m_roundCount)
	{
		m_players.GetMutex().lock();
		std::ranges::for_each(m_players.GetRef(), [&playersDone](const Player& player) {
			playersDone[player.GetName()] = false;
			});
		m_players.GetMutex().unlock();

		m_turn.SetTurnNumber(0);

		m_image.GetMutex().lock();
		m_image.GetRef().Clear();
		m_image.GetMutex().unlock();

		m_chat.GetMutex().lock();
		m_chat.GetRef().Clear();
		m_chat.GetMutex().unlock();

		for (auto currPlayerIt{ m_players.GetRef().begin() };
			currPlayerIt != m_players.GetRef().end();
			currPlayerIt = findNextPlayerLambda(m_players.GetRef()))
		{
			playersDone[currPlayerIt->GetName()] = true;
			m_turn.Reset(m_players, *currPlayerIt);

			m_gameState = common::game::GameState::PICK_WORD;

			while (!m_stopped && m_turn.GetWord() == "")
				std::this_thread::sleep_for(std::chrono::milliseconds{ 500 });

			if (m_stopped)
			{
				m_gameState = common::game::GameState::NONE;
				return;
			}

			m_gameState = common::game::GameState::DRAW_AND_GUESS;
			m_turn.Start(m_players, std::chrono::seconds{ m_gameSettings.m_drawTime }, m_stopped);

			RemoveDisconnectedPlayers();

			{
				LOCK(m_players.GetMutex());

				if (m_players.GetRef().empty())
				{
					m_gameState = common::game::GameState::NONE;
					m_stopped = true;
					return;
				}

				std::ranges::sort(m_players.GetRef(), std::greater<>{}, & Player::GetScore);
			}
		}

		m_roundNumber++;
	}

	m_gameState = common::game::GameState::NONE;
	m_stopped = true;
}

bool Game::ChatEmpty()
{
	LOCK(m_chat.GetMutex());
	return m_chat.GetRef().Empty();
}

crow::json::wvalue Game::GetMessagesOrderedJsonList(uint64_t start, const std::string& author)
{
	LOCK(m_chat.GetMutex());
	return m_chat.GetRef().GetMessagesOrderedJsonList(start, author);
}

void Game::SetPlayerGuessStatus(const std::string& username, bool status)
{
	LOCK(m_players.GetMutex());
	std::ranges::find_if(m_players.GetRef(), [&username, status](Player& player) {
		return player.GetName() == username; })
		->SetGuessStatus(status);
}

void Game::RemoveDisconnectedPlayers()
{
	m_players.GetRef().erase(
		std::remove_if(m_players.GetRef().begin(), m_players.GetRef().end(),
			[](const Player& player) { return !player.IsConnected(); }),
		m_players.GetRef().end());
}

void Game::Reset()
{
	m_gameState.store(common::game::GameState::NONE);
	m_roundNumber = 0;

	LOCK(m_players.GetMutex());
	RemoveDisconnectedPlayers();
}

void Game::Stop()
{
	m_stopped = true;
	while (m_gameState != common::game::GameState::NONE)
		std::this_thread::sleep_for(std::chrono::milliseconds{ 500 });
}

void Game::AppendImageUpdates(const std::vector<std::unordered_map<std::string, std::variant<int64_t, std::string>>>& jsonVector)
{
	const static std::string pointXStrKey{ literals::jsonKeys::draw::pointX };
	const static std::string pointYStrKey{ literals::jsonKeys::draw::pointY };
	const static std::string colorStrKey{ literals::jsonKeys::draw::color };

	LOCK(m_image.GetMutex());
	for (const auto& pointMap : jsonVector)
	{
		try
		{
			common::img::Point point{
				std::get<int64_t>(pointMap.at(pointXStrKey)),
				std::get<int64_t>(pointMap.at(pointYStrKey)),
				std::get<int64_t>(pointMap.at(colorStrKey))
			};

			m_image.GetRef().AddUpdate(common::img::Update{ point, utils::MillisFromDateTime(std::chrono::system_clock::now()) });
		}
		catch (const std::exception& exception)
		{
			throw exception;
		}
	}
}

void Game::RemovePlayer(const std::string& name)
{
	LOCK(m_players.GetMutex());
	m_players.GetRef().erase(std::remove_if(m_players.GetRef().begin(), m_players.GetRef().end(),
		[&name](const Player& player) {
			return player.GetName() == name;
		}), m_players.GetRef().end());
}

void Game::AddPlayer(const Player& player)
{
	LOCK(m_players.GetMutex());
	m_players.GetRef().emplace_back(player);
}

void Game::AddPlayer(Player&& player)
{
	LOCK(m_players.GetMutex());
	m_players.GetRef().emplace_back(std::move(player));
}
