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
	uint8_t GetTurnNumber() const;
	float_t GetChoiceTime() const;
	float_t GetPlayTime() const;
	std::string GetWord() const;
	std::vector<std::string> generateWordChoices(const uint8_t wordsCount);
	void sendWordChoices(const Player& player, const std::vector<std::string>& wordChoices) const;
	void choosingWordPhase(const std::vector<Player>& players);
	void startNewTurn(std::vector<Player>& players);
	void endTurn(std::vector<Player>& players);
	void startNextTurn(std::vector<Player>& players);

	static const std::string s_wordsFilePath;

private:
	uint8_t m_turnNumber;
	std::string m_word;
	std::vector<uint8_t> m_showLetterIDs;
	clock_t m_playStartTime;
	clock_t m_chooseWordStartTime;
	std::ifstream m_fin;
};
