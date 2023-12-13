#include "Game.h"

Game::Game(Game&& other) noexcept :
	m_players{ std::move(other.m_players) },
	m_roundNumber{ other.m_roundNumber },
	m_playerToDrawID{ other.m_playerToDrawID },
	m_ownerID{ other.m_ownerID },
	m_gameSettings{ std::move(other.m_gameSettings) },
	m_turn{ std::move(other.m_turn) },
	m_image{ std::move(other.m_image) },
	m_chat{ std::move(other.m_chat) }
{
	other.m_roundNumber = 0;
	other.m_playerToDrawID = 0;
	other.m_ownerID = 0;
}

Game& Game::operator=(Game&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}
	m_players = std::move(other.m_players);
	m_roundNumber = std::move(other.m_roundNumber);
	m_playerToDrawID = std::move(other.m_playerToDrawID);
	m_ownerID = std::move(other.m_ownerID);
	m_turn = std::move(other.m_turn);
	m_gameSettings = std::move(other.m_gameSettings);
	m_image = std::move(other.m_image);
	m_chat = std::move(other.m_chat);

	other.m_roundNumber = 0;
	other.m_playerToDrawID = 0;
	other.m_ownerID = 0;

	return *this;
}

const std::vector<Player>& Game::GetPlayers()
{
	return m_players;
}

uint8_t Game::GetRoundNumber()
{
	return m_roundNumber;
}

void Game::SetRoundNumber(uint8_t roundNo)
{
	m_roundNumber = roundNo;
}

uint8_t Game::GetPlayerToDrawID()
{
	return m_playerToDrawID;
}

void Game::SetPlayerToDrawID(uint8_t playerToDrawID)
{
	m_playerToDrawID = playerToDrawID;
}

GameSettings& Game::GetGameSettings()
{
	return m_gameSettings;
}

Turn& Game::GetTurn()
{
	return m_turn;
}

Image& Game::GetImage()
{
	return m_image;
}

Chat& Game::GetChat()
{
	return m_chat;
}

void Game::Start()
{

}

void Game::AddPlayer(const Player& player)
{
	m_players.push_back(player);
}

void Game::RemovePlayer(const std::string& name)
{
	for (auto& player : m_players)
		if (player.GetName() == name)
		{
			m_players.erase(std::remove(m_players.begin(), m_players.end(), player), m_players.end());
			return;
		}
}
