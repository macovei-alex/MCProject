#include "GameSettings.h"

#include <algorithm>

const uint16_t GameSettings::defaultChooseWordOptionCount = 3;
const uint16_t GameSettings::defaultDrawTime = 120;
const uint16_t GameSettings::defaultRoundCount = 4;
 
GameSettings::GameSettings() noexcept :
	m_roundCount{ defaultRoundCount },
	m_drawTime{ defaultDrawTime },
	m_chooseWordOptionCount{ defaultChooseWordOptionCount }
{
	/* empty */
}

GameSettings::GameSettings(uint8_t roundCount, uint8_t drawTime, uint8_t chooseWordOtionCount) noexcept :
m_roundCount{ roundCount },
	m_drawTime{ drawTime },
	m_chooseWordOptionCount{ chooseWordOtionCount }
{
	/* empty */
}

uint16_t GameSettings::GetRoundCount() const noexcept
{
	return m_roundCount;
}

uint16_t GameSettings::GetDrawTime() const noexcept
{
	return m_drawTime;
}

uint16_t GameSettings::GetChooseWordOptionCount() const noexcept
{
	return m_chooseWordOptionCount;
}

void GameSettings::SetRoundCount(uint16_t roundCount) noexcept
{
	m_roundCount = roundCount;
}

void GameSettings::SetDrawTime(uint16_t drawTime) noexcept
{
	m_drawTime = drawTime;
}

void GameSettings::SetChooseWordOptionCount(uint16_t chooseWordOptionCount) noexcept
{
	m_chooseWordOptionCount = chooseWordOptionCount;
}
