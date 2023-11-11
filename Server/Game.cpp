#include "Game.h"

void Game::start() 
{

}

void Game::pause() {}

void Game::removePlayer(uint8_t ID)
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

void Game::setPlayers(std::vector<Player> players)
{
	m_players = players;
}

std::vector<Player>& Game::getPlayers()
{
	return m_players;
}

void Game::setRoundNo(uint8_t roundNo)
{
	m_roundNumber = roundNo;
}

uint8_t Game::getRoundNo()
{
	return m_roundNumber;
}

void Game::setPlayerToDrawID(uint8_t playerToDrawID)
{
	m_playerToDrawID = playerToDrawID;
}

uint8_t Game::getPlayerToDrawID()
{
	return m_playerToDrawID;
}

std::vector<std::string> Game::getShownWords()
{
	return m_shownWords;
}

GameSettings& Game::getGameSettings()
{
	return m_gameSettings;
}

Turn& Game::getTurn()
{
	return m_turn;
}
