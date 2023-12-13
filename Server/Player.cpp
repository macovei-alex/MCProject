#include "Player.h"

Player::Player() :
	m_name{},
	m_score{ 0 },
	m_flagGuessedCorrectWord{ false },
	m_currentScore{ 0 },
	m_gameRole{ Player::GameRole::guessing },
	m_roomRole{ Player::RoomRole::participant }
{
	/*EMPTY*/
}

Player::Player(const std::string& name) :
	m_name{ name },
	m_score{ 0 },
	m_flagGuessedCorrectWord{ false },
	m_currentScore{ 0 },
	m_gameRole{ Player::GameRole::guessing },
	m_roomRole{ Player::RoomRole::participant }
{
	/*EMPTY*/
}

std::string Player::GetName() const
{
	return m_name;
}

int Player::GetScore() const
{
	return m_score;
}

int Player::GetCurrentScore() const
{
	return m_currentScore;
}

void Player::ResetCurrentScore()
{
	m_currentScore = 0;
}

void Player::ResetScore()
{
	m_score = 0;
}

Player::GameRole Player::GetGameRole() const
{
	return m_gameRole;
}

void Player::SetGameRole(GameRole gameRole)
{
	m_gameRole = gameRole;
}

Player::RoomRole Player::GetRoomRole() const
{
	return m_roomRole;
}

void Player::SetRoomRole(RoomRole roomRole)
{
	m_roomRole = roomRole;
}

void Player::AddScore()
{
	m_score += m_currentScore;
}

int Player::CalculateScore(int seconds, uint32_t maxSeconds, int playerCount)
{
	if (m_gameRole == Player::GameRole::drawing)
		return CalculateScoreDrawingPlayer(seconds, maxSeconds, playerCount);
	return CalculateScoreGuessingPlayer(seconds, maxSeconds);
}

int Player::CalculateScoreDrawingPlayer(int seconds, uint32_t maxSeconds, int playerCount)
{
	if (playerCount == 0)
	{
		m_currentScore += -100;
	}
	else
	{
		m_currentScore += ((maxSeconds - (seconds / playerCount)) * 100) / maxSeconds;
	}
	return m_currentScore;
}

int Player::CalculateScoreGuessingPlayer(int seconds, uint32_t maxSeconds)
{
	if (seconds < maxSeconds / 2)
	{
		m_currentScore += 100;
	}
	else if (seconds < maxSeconds)
	{
		m_currentScore += ((maxSeconds - seconds) * 100) / maxSeconds;
	}
	else
	{
		m_currentScore += -50;
	}

	return m_currentScore;
}

bool Player::operator==(const Player& other) const
{
	return m_name == other.m_name;
}
