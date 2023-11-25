#include "Game.h"

void Game::Start()
{

}

void Game::Pause()
{

}

void Game::RemovePlayer(uint8_t ID)
{
	for (int i = 0; i < m_players.size(); i++)
	{
		/*
		if (m_players[i].m_ID == ID)
		{
			m_players.erase(i);
		}
		*/
	}
}

Game::Game(const std::vector<Player>& players, uint8_t roundNumber, uint8_t playerToDrawID, uint8_t ownerID, const std::vector<std::string>& shownWords, GameSettings gameSettings, Turn turn) :
	m_players{ players },
	m_roundNumber{ roundNumber },
	m_playerToDrawID{ playerToDrawID },
	m_ownerID{ ownerID },
	m_shownWords{ shownWords },
	m_gameSettings{ gameSettings },
	m_turn{ turn }
{}

Game::Game(const Game& game):
	m_players{ game.m_players },
	m_roundNumber{ game.m_roundNumber },
	m_playerToDrawID{ game.m_playerToDrawID },
	m_ownerID{ game.m_ownerID },
	m_shownWords{ game.m_shownWords },
	m_gameSettings{ game.m_gameSettings },
	m_turn{ game.m_turn }
{}

Game::~Game() 
{
	/*EMPTY*/
}

Game& Game::operator=(const Game& game)
{
	m_players = game.m_players;
	m_roundNumber = game.m_roundNumber;
	m_playerToDrawID = game.m_playerToDrawID;
	m_ownerID = game.m_ownerID;
	m_shownWords = game.m_shownWords;
	m_turn = game.m_turn;
	m_gameSettings = game.m_gameSettings;
	return *this;
}

Game::Game(Game&& other) noexcept :
	m_players{ std::move(other.m_players) },
	m_roundNumber{ other.m_roundNumber },
	m_playerToDrawID{ other.m_playerToDrawID },
	m_ownerID{ other.m_ownerID },
	m_shownWords{ std::move(other.m_shownWords)},
	m_gameSettings{ std::move(other.m_gameSettings) },
	m_turn{ std::move(other.m_turn) }
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
	m_shownWords = std::move(other.m_shownWords);
	m_turn = std::move(other.m_turn);
	m_gameSettings = std::move(other.m_gameSettings);

	other.m_roundNumber = 0;
	other.m_playerToDrawID = 0;
	other.m_ownerID = 0;

	return *this;
}

void Game::SetPlayers(const std::vector<Player>& players)
{
	m_players = players;
}

std::vector<Player>& Game::GetPlayers()
{
	return m_players;
}

void Game::SetRoundNo(uint8_t roundNo)
{
	m_roundNumber = roundNo;
}

uint8_t Game::GetRoundNo()
{
	return m_roundNumber;
}

void Game::SetPlayerToDrawID(uint8_t playerToDrawID)
{
	m_playerToDrawID = playerToDrawID;
}

uint8_t Game::GetPlayerToDrawID()
{
	return m_playerToDrawID;
}

std::vector<std::string> Game::GetShownWords()
{
	return m_shownWords;
}

GameSettings& Game::GetGameSettings()
{
	return m_gameSettings;
}

Turn Game::GetTurn()
{
	return m_turn;
}
