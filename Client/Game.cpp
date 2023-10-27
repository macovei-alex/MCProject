#include "Game.h"

void Game::start() {}

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

Game::Game(std::vector<Player> players, uint8_t roundNo, uint8_t playerToDrawID, 
	uint8_t ownerID, std::vector<const std::string&> shownWords, std::vector<GameSettings> gameSettings, Turn turn):
	m_players(players),
	m_roundNo(roundNo),
	m_playerToDrawID(playerToDrawID),
	m_ownerID(ownerID),
	m_shownWords(shownWords),
	m_gameSettings(gameSettings),
	m_turn(turn)
{}

Game::Game(const Game& game):
	m_players(game.m_players),
	m_roundNo(game.m_roundNo),
	m_playerToDrawID(game.m_playerToDrawID),
	m_ownerID(game.m_ownerID),
	m_shownWords(game.m_shownWords),
	m_gameSettings(game.m_gameSettings),
	m_turn(game.m_turn)
{}

Game::~Game() {}

Game& Game::operator=(const Game& game)
{
	m_players = game.m_players;
	m_roundNo = game.m_roundNo;
	m_playerToDrawID = game.m_playerToDrawID;
	m_ownerID = game.m_ownerID;
	m_shownWords = game.m_shownWords;
	m_turn = game.m_turn;
	m_gameSettings = game.m_gameSettings;
	return *this;
}