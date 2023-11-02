#pragma once
#include <cstdint>
#include <string>

class Player
{
public:
	Player();
	Player(const std::string& m_name);
	Player(const Player& player);
	Player& operator= (const Player& player);
	~Player();
	std::string GetName() const;
	uint16_t GetScore() const;
	uint16_t GetCurrentScore() const;
	void resetCurrentScore();
	void resetScore();
	void addScore();
	uint16_t calculateScoreDrawingPlayer(uint16_t seconds, uint8_t playerCount);
	uint16_t calculateScoreGuessingPlayer(uint16_t seconds);

	enum class GameRole : uint8_t
	{
		person_drawing,
		person_guessing
	};

	enum class RoomRole : uint8_t
	{
		owner,
		participants
	};

	GameRole GetGameRole() const;
	RoomRole GetRoomRole() const;

private:
	std::string m_name;
	uint16_t m_score;
	uint16_t m_currentScore;
	bool m_flagGuessedCorrectWord;
	RoomRole m_roomRole : 1;
	GameRole m_gameRole: 1;
};

