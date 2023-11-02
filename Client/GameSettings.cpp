#include "GameSettings.h"
const uint16_t GameSettings::defaultChooseWordOptionCount = 3;
const uint16_t GameSettings::defaultDrawTime = 120;
const uint16_t GameSettings::defaultRoundCount = 4;
 
GameSettings::GameSettings() :
	m_roundCount{ defaultRoundCount },
	m_drawTime{ defaultDrawTime },
	m_chooseWordOptionCount{ defaultChooseWordOptionCount }
{
	//EMPTY
};

GameSettings::GameSettings(uint8_t roundCount, uint8_t drawTime, uint8_t chooseWordOtionCount):
	m_roundCount{ roundCount },
	m_drawTime{ drawTime },
	m_chooseWordOptionCount{ chooseWordOtionCount }
{
	//EMPTY
}

GameSettings& GameSettings::operator=(const GameSettings& other)
{
	if (this == &other) {
		// Evit?m auto-atribuirea
		return *this;
	}

	// Copiem valorile din obiectul "other" în obiectul curent
	m_roundCount = other.m_roundCount;
	m_drawTime = other.m_drawTime;
	m_chooseWordOptionCount = other.m_chooseWordOptionCount;

	return *this;
}

uint16_t GameSettings::GetRoundCount()
{
	return m_roundCount;
}

uint16_t GameSettings::GetDrawTime()
{
	return m_drawTime;
}

uint16_t GameSettings::GetChooseWordOptionCountt()
{
	return m_chooseWordOptionCount;
}


GameSettings::GameSettings(const GameSettings& other) :
	m_roundCount{ defaultRoundCount },
	m_drawTime{ defaultDrawTime },
	m_chooseWordOptionCount{ defaultChooseWordOptionCount }
{
	//EMPTY
};

