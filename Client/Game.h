#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "Turn.h"

class Game
{
public:

	void start();
	void pause();
	void removePlayer();

private:

	std::vector<Player> m_players;
	uint16_t m_roundNo;
	uint16_t m_playerToDrawID;
	uint16_t m_ownerID;
	std::vector<const std::string&> m_shownWords;
	std::vector<GameSettings> m_gameSettings;
	Turn m_turn;

};

