#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <random>
#include "Player.h"

class Turn
{
public:
	Turn(uint8_t turnNo, const std::string& word, int8_t turnToDraw);
	Turn(Turn& turn);
	Turn& operator=(Turn& turn);
	~Turn();
	uint8_t GetTurnNumber() const;
	double_t GetChooseTime() const;
	double_t GetPlayTime() const;
	std::string GetWord() const;
	std::vector<std::string> generateWordChoices(const size_t wordsCount);
	std::string pickWord(const std::vector<std::string>& words, const size_t wordsCount) const;
	void startNewTurn(std::vector<Player>& players);
	void endTurn(std::vector<Player>& players);

	static const std::string wordsFilePath;

private:
	uint8_t m_turnNumber;
	std::string m_word;
	int8_t m_turnToDraw;
	std::vector<uint8_t> m_showLetterIDs;
	clock_t m_startTime;
	clock_t m_chooseWordStartTime;
	std::ifstream m_fin;

	static std::random_device seed;
	static std::mt19937 randomNumberGenerator;
};
