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
	void Start();
	void Pause();
	void RemovePlayer(uint8_t ID);
	Game(const std::vector<Player>& players, uint8_t roundNumber, uint8_t playerToDrawID, uint8_t ownerID, const std::vector<std::string>& shownWords, GameSettings gameSettings, Turn turn);
	Game(const Game& game);
	~Game();
	Game& operator=(const Game& game);
	Game(Game&& other) noexcept;
	Game& operator= (Game&& other) noexcept;
	void SetPlayers(const std::vector<Player>& players);
	std::vector<Player>& GetPlayers();
	void SetRoundNo(uint8_t roundNo);
	uint8_t GetRoundNo();
	void SetPlayerToDrawID(uint8_t playerToDrawID);
	uint8_t GetPlayerToDrawID();
	void AddShownWords(const std::vector<std::string>& words);
	std::vector<std::string> GetShownWords();
	GameSettings& GetGameSettings();
	Turn GetTurn();

private:
	std::vector<Player> m_players;
	uint8_t m_roundNumber;
	uint8_t m_playerToDrawID;
	uint8_t m_ownerID;
	std::vector<std::string> m_shownWords;
	GameSettings m_gameSettings;
	Turn m_turn;
};