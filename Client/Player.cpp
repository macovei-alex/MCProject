#include "Player.h"

Player::Player() :
	m_name{ "" },
	m_score{ 0 },
	m_flagGuessedCorrrectWord{ false },
	m_currentScore{ 0 }
{}

Player::Player(const std::string& name) :
	m_name{ name },
	m_score{ 0 },
	m_flagGuessedCorrrectWord{ false },
	m_currentScore{ 0 }
{
	/*EMPTY*/
}

Player::Player(const Player& player) :
	m_name{ player.m_name },
	m_score{ player.m_score },
	m_flagGuessedCorrrectWord{ player.m_flagGuessedCorrrectWord },
	m_currentScore{ 0 }
{
	/*EMPTY*/
}

Player& Player::operator=(const Player& player)
{
	m_name = player.m_name;
	m_score = player.m_score;
	m_currentScore = player.m_currentScore;
	m_flagGuessedCorrrectWord = player.m_flagGuessedCorrrectWord;
	return *this;

}

Player::~Player()
{
	/*EMPTY*/
}

std::string Player::GetName() const
{
	return m_name;
}

uint16_t Player::GetScore() const
{
	return m_score;
}

uint16_t Player::GetCurrentScore() const
{
	return m_currentScore;
}

void Player::SetCurrentScore()
{
	m_currentScore = 0;
}

void Player::SetScore()
{
	m_score = 0;
}

void Player::guessWord(std::string guessedWord, Turn turn)
{
	if (guessedWord == turn.GetWord()) m_flagGuessedCorrrectWord = true;
}

void Player::addScore()
{
	m_score += m_currentScore;
}

