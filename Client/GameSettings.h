#pragma once
#include<cstdint>
class GameSettings
{ 
public:
	GameSettings();
	GameSettings(const GameSettings& other);
	GameSettings(uint8_t roundCount, uint8_t drawTime, uint8_t chooseWordOtionCount);
	const uint8_t m_roundCount;
	const uint8_t m_drawTime;
	const  uint8_t m_chooseWordOptionCount;
private:
	 static int const defaultRoundCount;
	 static int const defaultDrawTime ;
	 static int const defaultChooseWordOptionCount;
};

