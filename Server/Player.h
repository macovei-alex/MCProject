#pragma once

#include <cstdint>
#include <string>

#include "common.h"

class Player
{
public:

	enum class RoomRole : uint8_t
	{
		owner,
		participant
	};

public:
	Player() noexcept;
	Player(const std::string& m_name) noexcept;
	Player(const Player& other) noexcept = default;
	Player& operator=(const Player& other) noexcept = default;
	~Player() noexcept = default;
	Player(Player&& other) noexcept = default;
	Player& operator=(Player&& other) noexcept = default;

public:
	std::string GetName() const noexcept;
	int GetScore() const noexcept;
	void ResetScore() noexcept;
	int GetCurrentScore() const noexcept;
	void ResetCurrentScore() noexcept;
	common::game::PlayerRole GetRole() const noexcept;
	void SetGameRole(common::game::PlayerRole gameRole) noexcept;
	RoomRole GetRoomRole() const noexcept;
	void SetRoomRole(RoomRole roomRole) noexcept;

public:
	void AddScore();
	int CalculateScore(uint16_t seconds, uint32_t maxSeconds, int playerCount = 1);
	int CalculateScoreDrawingPlayer(uint16_t seconds, uint32_t maxSeconds, int playerCount);
	int CalculateScoreGuessingPlayer(uint16_t seconds, uint32_t maxSeconds);
	bool operator==(const Player& other) const noexcept;

private:
	std::string m_name;
	int m_score;
	int m_currentScore;
	bool m_flagGuessedCorrectWord;
	RoomRole m_roomRole : 2;
	common::game::PlayerRole m_gameRole : 1;
};

