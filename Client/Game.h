#pragma once

#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include "Player.h"
#include "Turn.h"
#include "GameSettings.h"

class Game
{
public:
	void start();
	void pause();
	void removePlayer(uint8_t ID);
	Game(std::vector<Player> players, uint8_t roundNo, uint8_t playerToDrawID,
		uint8_t ownerID,
		std::vector<const std::string&> shownWords,
		std::vector<GameSettings> gameSettings,
		Turn turn);
	Game(const Game& game);
	~Game();
	Game& operator=(const Game& game);
	void setPlayers(std::vector<Player> players);
	void setRoundNo(uint8_t roundNo);
	void setPlayerToDrawID(uint8_t playerToDrawID);
	void setOwnerID(uint8_t ownerID);
	void setShownWords(std::vector<const std::string&> shownWords);
	void setGameSettings(std::vector<GameSettings> gameSettings);
	void setTurn(Turn turn);

private:
	std::vector<Player> m_players;
	uint8_t m_roundNo;
	uint8_t m_playerToDrawID;
	uint8_t m_ownerID;
	std::vector<const std::string&> m_shownWords;
	std::vector<GameSettings> m_gameSettings;
	Turn m_turn;
};