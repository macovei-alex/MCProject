#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <ctime>
#include "Player.h"

class Turn
{
public:
	Turn(uint8_t turnNo, const std::string& word, int8_t turnToDraw);
	Turn(const Turn& turn);
	Turn& operator=(const Turn& turn);
	~Turn();
	uint8_t GetTurnNumber() const;
	double_t GetTime() const;
	std::string GetWord() const;
	std::string generateWord() const;
	void startNewTurn(const std::vector<Player>& players);
	void endTurn(const std::vector<Player>& players);

private:
	uint8_t m_turnNumber;
	std::string m_word;
	int8_t m_turnToDraw;
	std::vector<uint8_t> m_showLetterIDs;
	clock_t m_startTime;
};
