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
//	GameSettings& operator =(const GameSettings& other);
private:
	 // std::string customWords// vector de stringuri
	//static vector de string public constant, initializez constructor 
	// constructor de copiere
	 static int const defaultRoundCount;
	 static int const defaultDrawTime ;
	 static int const defaultChooseWordOptionCount;

};

