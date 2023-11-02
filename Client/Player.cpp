#include "Player.h"

Player::Player() :
	m_name{ "" },
	m_score{ 0 },
	m_flagGuessedCorrectWord{ false },
	m_currentScore{ 0 },
	m_gameRole{ Player::GameRole::person_guessing },
	m_roomRole{ Player::RoomRole::participants }
{
	/*EMPTY*/
}

Player::Player(const std::string& name) :
	m_name{ name },
	m_score{ 0 },
	m_flagGuessedCorrectWord{ false },
	m_currentScore{ 0 },
	m_gameRole{ Player::GameRole::person_guessing },
	m_roomRole{ Player::RoomRole::participants }
{
	/*EMPTY*/
}

Player::Player(const Player& player) :
	m_name{ player.m_name },
	m_score{ player.m_score },
	m_flagGuessedCorrectWord{ player.m_flagGuessedCorrectWord },
	m_currentScore{ 0 },
	m_gameRole{ Player::GameRole::person_guessing },
	m_roomRole{ Player::RoomRole::participants }
{
	/*EMPTY*/
}

Player& Player::operator=(const Player& player)
{
	m_name = player.m_name;
	m_score = player.m_score;
	m_currentScore = player.m_currentScore;
	m_flagGuessedCorrectWord = player.m_flagGuessedCorrectWord;
	m_gameRole = player.m_gameRole;
	m_roomRole = player.m_roomRole;
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

uint16_t Player::calculateScoreDrawingPlayer(uint16_t seconds, uint8_t playerCount)
{
	if (playerCount == 0)
	{
		m_currentScore += -100;
	}
	else
	{
		m_currentScore += ((60 - (seconds / playerCount)) * 100) / 60;
	}
	return m_currentScore;
}

uint16_t Player::calculateScoreGuessingPlayer(uint16_t seconds)
{
	if (seconds < 30)
	{
		m_currentScore += 100;
	}
	else if (seconds < 60)
	{
		m_currentScore += ((60 - seconds) * 100) / 30;
	}
	else
	{
		m_currentScore += -50;
	}

	return m_currentScore;
}

Player::GameRole Player::GetGameRole() const
{
	return m_gameRole;
}

Player::RoomRole Player::GetRoomRole() const
{
	return m_roomRole;
}
