#include "Player.h"

Player::Player(const std::string& name) :
	m_name(name),
	m_score(0),
	m_flagGuessedCorrrectWord(false)
{
	/*EMPTY*/
}

Player::Player(const Player& player) :
	m_name(player.m_name),
	m_score(player.m_score),
	m_flagGuessedCorrrectWord(player.m_flagGuessedCorrrectWord)
{
	/*EMPTY*/
}

Player::~Player()
{
	/*EMPTY*/
}

uint16_t Player::GetScore()
{
	return m_score;
}

uint16_t Player::GetCurrentScore()
{
	return m_currentScore;
}

void Player::SetCurrentScore()
{
	m_currentScore = 0;
}

void Player::guessWord(std::string guessedWord, Turn turn)
{
	if (guessedWord == turn.GetCorrectWord()) m_flagGuessedCorrrectWord = true;
}

void Player::addScore()
{
	m_score += m_currentScore;
}
