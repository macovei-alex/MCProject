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
	Game() noexcept = default;
	Game(const Game& other) noexcept = default;
	Game& operator=(const Game& other) noexcept = default;
	Game(Game&& other) noexcept;
	Game& operator= (Game&& other) noexcept;
	~Game() noexcept = default;

public:
	const std::vector<Player>& GetPlayers();
	void SetRoundNumber(uint8_t roundNumber);
	uint8_t GetRoundNumber();
	void SetPlayerToDrawID(uint8_t playerToDrawID);
	uint8_t GetPlayerToDrawID();
	const GameSettings& GetGameSettings();
	Turn& GetTurn();

public:
	void Start();
	void AddPlayer(const Player& player);
	void RemovePlayer(const std::string& playerName);

private:
	std::vector<Player> m_players;
	uint8_t m_roundNumber;
	uint8_t m_playerToDrawID;
	uint8_t m_ownerID;
	Turn m_turn;
	GameSettings m_gameSettings;
};