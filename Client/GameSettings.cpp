#include "GameSettings.h"
GameSettings::GameSettings() :
	m_roundCount(defaultRoundCount),
	m_drawTime(defaultDrawTime),
	m_chooseWordOptionCount(defaultChooseWordOptionCount)
{
	//EMPTY
};

GameSettings::GameSettings(uint8_t roundCount, uint8_t drawTime, uint8_t chooseWordOtionCount):
	m_roundCount(roundCount),
	m_drawTime(drawTime),
	m_chooseWordOptionCount(chooseWordOtionCount)
{
	//EMPTY
}

//GameSettings& GameSettings::operator=(const GameSettings& other)
//{
//	{
//		if (this == &other) {
//			return *this;
//		}
//		m_roundCount = other.m_roundCount;
//		m_drawTime = other.m_drawTime;
//		m_chooseWordOptionCount = other.m_chooseWordOptionCount;
//
//	}
//}
GameSettings::GameSettings(const GameSettings& other) :
	m_roundCount(defaultRoundCount),
	m_drawTime(defaultDrawTime),
	m_chooseWordOptionCount(defaultChooseWordOptionCount)
{
	//EMPTY
};

