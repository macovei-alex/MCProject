#include "Player.h"

Player::Player() :
	m_name{ "" },
	m_score{ 0 },
	m_flagGuessedCorrectWord{ false },
	m_currentScore{ 0 }
{}

Player::Player(const std::string& name) :
	m_name{ name },
	m_score{ 0 },
	m_flagGuessedCorrectWord{ false },
	m_currentScore{ 0 }
{
	/*EMPTY*/
}

Player::Player(const Player& player) :
	m_name{ player.m_name },
	m_score{ player.m_score },
	m_flagGuessedCorrectWord{ player.m_flagGuessedCorrectWord },
	m_currentScore{ 0 }
{
	/*EMPTY*/
}

Player& Player::operator=(const Player& player)
{
	m_name = player.m_name;
	m_score = player.m_score;
	m_currentScore = player.m_currentScore;
	m_flagGuessedCorrectWord = player.m_flagGuessedCorrectWord;
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

void Player::resetCurrentScore()
{
	m_currentScore = 0;
}

void Player::resetScore()
{
	m_score = 0;
}

void Player::addScore()
{
	m_score += m_currentScore;
}

