#pragma once

#include <cstdint>
#include <string>

class Player
{
public:
	enum class GameRole : uint8_t
	{
		drawing,
		guessing
	};

	enum class RoomRole : uint8_t
	{
		owner,
		participant
	};

public:
	Player();
	Player(const std::string& m_name);
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
	GameRole GetGameRole() const noexcept;
	void SetGameRole(GameRole gameRole) noexcept;
	RoomRole GetRoomRole() const noexcept;
	void SetRoomRole(RoomRole roomRole) noexcept;

public:
	void AddScore();
	int CalculateScore(int seconds, uint32_t maxSeconds, int playerCount = 0);
	int CalculateScoreDrawingPlayer(int seconds, uint32_t maxSeconds, int playerCount);
	int CalculateScoreGuessingPlayer(int seconds, uint32_t maxSeconds);
	bool operator==(const Player& other) const;

private:
	std::string m_name;
	int m_score;
	int m_currentScore;
	bool m_flagGuessedCorrectWord;
	RoomRole m_roomRole : 1;
	GameRole m_gameRole : 1;
};

