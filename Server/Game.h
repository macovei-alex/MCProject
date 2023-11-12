#pragma once

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
	Game(const std::vector<Player>& players, uint8_t roundNumber, uint8_t playerToDrawID, uint8_t ownerID, const std::vector<std::string>& shownWords, GameSettings gameSettings, Turn turn);
	Game(const Game& game);
	~Game();
	Game& operator=(const Game& game);
	Game(Game&& other) noexcept;
	Game& operator= (Game&& other) noexcept;
	void setPlayers(std::vector<Player> players);
	std::vector<Player>& getPlayers();
	void setRoundNo(uint8_t roundNo);
	uint8_t getRoundNo();
	void setPlayerToDrawID(uint8_t playerToDrawID);
	uint8_t getPlayerToDrawID();
	void addShownWords(const std::vector<std::string>& words);
	std::vector<std::string> getShownWords();
	GameSettings& getGameSettings();
	Turn& getTurn();

private:
	std::vector<Player> m_players;
	uint8_t m_roundNumber;
	uint8_t m_playerToDrawID;
	uint8_t m_ownerID;
	std::vector<std::string> m_shownWords;
	GameSettings m_gameSettings;
	Turn m_turn;
};