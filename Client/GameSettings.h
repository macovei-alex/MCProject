#pragma once
#include<cstdint>
class GameSettings
{ 
public:

	GameSettings();
	void chooseSettings();

private:

	 uint8_t m_roundCount;
	 uint8_t m_drawTime;
	 uint8_t chooseWordOptionCount;
	 // std::string customWords;
	 static int const defaultRoundCount = 0;
	 static int const defaultDrawTime = 80;
	 static int const defaultChooseWordOptionCount = 2;

};

