#pragma once
#include <cstdint>
#include <string>
#include "Turn.h"

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
	void SetCurrentScore();
	void SetScore();
	void guessWord(std::string guessedWord, Turn turn);
	void addScore();

private:
	std::string m_name;
	uint16_t m_score;
	uint16_t m_currentScore;
	bool m_flagGuessedCorrrectWord;
};

