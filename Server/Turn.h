#pragma once

#include "Player.h"
#include "GameSettings.h"

#include <cstdint>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>

class Turn
{
public:
	Turn(uint8_t turnNumber);
	Turn(const Turn& turn);
	Turn& operator=(const Turn& turn);
	~Turn();
	uint8_t GetTurnNumber() const noexcept;
	float_t GetChoiceTime() const noexcept;
	float_t GetPlayTime() const noexcept;
	std::string GetWord() const noexcept;
	void ChoosingWordPhase(const std::vector<Player>& players);
	void StartNewTurn(std::vector<Player>& players);
	void EndTurn(std::vector<Player>& players);
	void StartNextTurn(std::vector<Player>& players);

private:
	uint8_t m_turnNumber;
	std::string m_word;
	std::vector<uint8_t> m_showLetterIDs;
	clock_t m_playStartTime;
	clock_t m_chooseWordStartTime;
};
