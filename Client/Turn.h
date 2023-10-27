#pragma once
#include <cstdint>
#include <string>
#include <vector>

class Turn
{
public:

	Turn(uint8_t turnNo, const std::string& word, uint8_t turnToDraw);
	Turn(const Turn& turn);
	Turn& operator=(const Turn& turn);
	~Turn();

private:

	uint8_t m_turnNo;
	std::string m_word;
	uint8_t m_turnToDraw;
	std::vector<uint8_t> m_showLetterIDs;
	// timer

};

