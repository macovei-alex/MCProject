#include "Player.h"

Player::Player() noexcept :
	m_name{},
	m_score{ 0 },
	m_guessedStatus{ false },
	m_currentScore{ 0 },
	m_gameRole{ common::game::PlayerRole::NONE },
	m_roomRole{ Player::RoomRole::participant },
	m_isConnected{ true }
{
	/*EMPTY*/
}

Player::Player(const std::string& name) noexcept :
	m_name{ name },
	m_score{ 0 },
	m_guessedStatus{ false },
	m_currentScore{ 0 },
	m_gameRole{ common::game::PlayerRole::NONE },
	m_roomRole{ Player::RoomRole::participant },
	m_isConnected{ true }
{
	/*EMPTY*/
}

std::string Player::GetName() const noexcept
{
	return m_name;
}

int Player::GetScore() const noexcept
{
	return m_score;
}

int Player::GetCurrentScore() const noexcept
{
	return m_currentScore;
}

void Player::ResetCurrentScore() noexcept
{
	m_currentScore = INT_MIN;
}

void Player::ResetScore() noexcept
{
	m_score = 0;
}

common::game::PlayerRole Player::GetRole() const noexcept
{
	return m_gameRole;
}

void Player::SetGameRole(common::game::PlayerRole gameRole) noexcept
{
	m_gameRole = gameRole;
}

Player::RoomRole Player::GetRoomRole() const noexcept
{
	return m_roomRole;
}

void Player::SetRoomRole(RoomRole roomRole) noexcept
{
	m_roomRole = roomRole;
}

void Player::SetGuessStatus(bool status) noexcept
{
	m_guessedStatus = status;
}

bool Player::GetGuessStatus() const noexcept
{
	return m_guessedStatus;
}

void Player::SetConnectionStatus(bool status) noexcept
{
	m_isConnected = status;
}

bool Player::IsConnected() const noexcept
{
	return m_isConnected;
}

void Player::AddScore()
{
	m_score += m_currentScore;
}

int Player::CalculateScore(uint16_t seconds, uint32_t maxSeconds, int playerCount)
{
	if (m_gameRole == common::game::PlayerRole::DRAWING)
		return CalculateScoreDrawingPlayer(seconds, maxSeconds, playerCount);
	return CalculateScoreGuessingPlayer(seconds, maxSeconds);
}

int Player::CalculateScoreDrawingPlayer(uint16_t seconds, uint32_t maxSeconds, int playerCount)
{
	if (playerCount == 0)
	{
		m_currentScore = -100;
	}
	else
	{
		m_currentScore = ((maxSeconds - (seconds / playerCount)) * 100) / maxSeconds;
	}
	return m_currentScore;
}

int Player::CalculateScoreGuessingPlayer(uint16_t seconds, uint32_t maxSeconds)
{
	if (seconds < maxSeconds / 2)
	{
		m_currentScore = 100;
	}
	else if (seconds < maxSeconds)
	{
		m_currentScore = ((maxSeconds - seconds) * 100) / maxSeconds;
	}
	else
	{
		m_currentScore = -50;
	}

	return m_currentScore;
}

bool Player::operator==(const Player& other) const noexcept
{
	return m_name == other.m_name;
}
