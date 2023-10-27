#include "Game.h"

void Game::start()
{
}

void Game::pause()
{
}

void Game::removePlayer()
{
}

Game::Game(std::vector<Player> players, uint16_t roundNo, uint16_t playerToDrawID, 
	uint16_t ownerID, std::vector<const std::string&> shownWords, Turn turn):
	m_players(players),
	m_roundNo(roundNo),
	m_playerToDrawID(playerToDrawID),
	m_ownerID(ownerID),
	m_shownWords(shownWords),
	/*m_gameSettings(gameSettings)*/
	m_turn(turn)
{}

Game::Game(const Game& game):
	m_players(game.m_players),
	m_roundNo(game.m_roundNo),
	m_playerToDrawID(game.m_playerToDrawID),
	m_ownerID(game.m_ownerID),
	m_shownWords(game.m_shownWords),
	/*m_gameSettings(game.m_gameSettings)*/
	m_turn(game.m_turn)
{}

Game::~Game()
{
}

Game& Game::operator=(const Game& game)
{
	m_players = game.m_players;
	m_roundNo = game.m_roundNo;
	m_playerToDrawID = game.m_playerToDrawID;
	m_ownerID = game.m_ownerID;
	m_shownWords = game.m_shownWords;
	m_turn = game.m_turn;
	return *this;
}