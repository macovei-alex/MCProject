#include "GameSettings.h"
GameSettings::GameSettings():
	m_roundCount(defaultRoundCount),
	m_drawTime(defaultDrawTime),
	m_chooseWordOptionCount(defaultChooseWordOptionCount)
{
	//EMPTY
}

void GameSettings::chooseSettings()
{
	//EMPTY
}

uint8_t GameSettings::GetDrawTime() const
{
	return m_drawTime;
}

uint8_t GameSettings::GetRoundCount() const
{
	return m_roundCount;
}

uint8_t GameSettings::GetChooseWordOptionCount() const
{
	return m_chooseWordOptionCount;
}
