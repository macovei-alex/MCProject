#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Player.h"
#include "Turn.h"

class Game
{
public:

	void start();
	void pause();
	void removePlayer();
	Game(std::vector<Player> players, uint16_t roundNo, uint16_t playerToDrawID,
		uint16_t ownerID,
		std::vector<const std::string&> shownWords,
		/*std::vector<GameSettings> gameSettings;*/
		Turn turn);
	Game(const Game& game);
	~Game();
	Game& operator=(const Game& game);

private:

	std::vector<Player> m_players;
	uint16_t m_roundNo;
	uint16_t m_playerToDrawID;
	uint16_t m_ownerID;
	std::vector<const std::string&> m_shownWords;
	// std::vector<GameSettings> m_gameSettings;
	Turn m_turn;
};