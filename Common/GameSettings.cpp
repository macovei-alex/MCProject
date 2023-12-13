#include "GameSettings.h"

#include <algorithm>

const uint16_t GameSettings::defaultChooseWordOptionCount = 3;
const uint16_t GameSettings::defaultDrawTime = 120;
const uint16_t GameSettings::defaultRoundCount = 4;
 
GameSettings::GameSettings() :
	m_roundCount{ defaultRoundCount },
	m_drawTime{ defaultDrawTime },
	m_chooseWordOptionCount{ defaultChooseWordOptionCount }
{
	/* empty */
}

GameSettings::GameSettings(uint8_t roundCount, uint8_t drawTime, uint8_t chooseWordOtionCount) :
m_roundCount{ roundCount },
	m_drawTime{ drawTime },
	m_chooseWordOptionCount{ chooseWordOtionCount }
{
	/* empty */
}

uint16_t GameSettings::GetRoundCount() const
{
	return m_roundCount;
}

uint16_t GameSettings::GetDrawTime() const
{
	return m_drawTime;
}

uint16_t GameSettings::GetChooseWordOptionCount() const
{
	return m_chooseWordOptionCount;
}

void GameSettings::SetRoundCount(uint16_t roundCount)
{
	m_roundCount = roundCount;
}

void GameSettings::SetDrawTime(uint16_t drawTime)
{
	m_drawTime = drawTime;
}

void GameSettings::SetChooseWordOptionCount(uint16_t chooseWordOptionCount)
{
	m_chooseWordOptionCount = chooseWordOptionCount;
}
