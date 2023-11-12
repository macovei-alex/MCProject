#pragma once
#include<cstdint>
class GameSettings
{ 
public:
	GameSettings();
	GameSettings(const GameSettings& other);
	GameSettings(uint8_t roundCount, uint8_t drawTime, uint8_t chooseWordOtionCount);
	~GameSettings();
	GameSettings& operator=(GameSettings&& other) noexcept;
	GameSettings& operator=( const GameSettings& other);
	uint16_t GetRoundCount();
	uint16_t GetDrawTime();
	uint16_t GetChooseWordOptionCountt();

private:
	 static uint16_t const defaultRoundCount;
	 static uint16_t const defaultDrawTime ;
	 static uint16_t const defaultChooseWordOptionCount;
	 uint16_t m_roundCount;
	 uint16_t m_drawTime;
	 uint16_t m_chooseWordOptionCount;
};

