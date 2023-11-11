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
	Player(Player&& other) noexcept;
	Player& operator= (Player&& other) noexcept;
	std::string GetName() const;
	int GetScore() const;
	int GetCurrentScore() const;
	void resetCurrentScore();
	void resetScore();
	void addScore();
	int calculateScoreDrawingPlayer(int seconds, int playerCount);
	int calculateScoreGuessingPlayer(int seconds);

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
	void SetGameRole(GameRole gameRole);
	RoomRole GetRoomRole() const;
	void SetRoomRole(RoomRole roomRole);

private:
	std::string m_name;
	int m_score;
	int m_currentScore;
	bool m_flagGuessedCorrectWord;
	RoomRole m_roomRole : 1;
	GameRole m_gameRole: 1;
};

