#include "common.h"

#include <algorithm>

using namespace common::game;

const uint16_t GameSettings::s_defaultChooseWordOptionCount = 3;
const uint16_t GameSettings::s_defaultDrawTime = 120;
const uint16_t GameSettings::s_defaultRoundCount = 4;

GameSettings::GameSettings() noexcept :
	m_roundCount{ s_defaultRoundCount },
	m_drawTime{ s_defaultDrawTime },
	m_chooseWordOptionCount{ s_defaultChooseWordOptionCount }
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
