#pragma once

#include <vector>
#include <cstdint>
#include <string>

#include "Player.h"
#include "Turn.h"
#include "Image.h"
#include "Chat.h"
#include "common.h"

class Game
{
public:
	Game() noexcept;
	Game(const Game& other) noexcept = default;
	Game& operator=(const Game& other) noexcept = default;
	Game(Game&& other) noexcept;
	Game& operator=(Game&& other) noexcept;
	~Game() noexcept = default;

public:
	const std::vector<Player>& GetPlayers();
	const Player& GetPlayer(const std::string& name) const;
	uint8_t GetRoundNumber();
	void SetRoundNumber(uint8_t roundNumber);
	uint8_t GetPlayerToDrawID();
	void SetPlayerToDrawID(uint8_t playerToDrawID);
	common::game::GameSettings& GetGameSettings();
	Turn& GetTurn();
	Image& GetImage();
	Chat& GetChat();
	common::game::GameState GetGameState();
	void SetGameState(common::game::GameState gameState);

public:
	void Run();
	void AddPlayer(const Player& player);
	void RemovePlayer(const std::string& playerName);

private:
	uint8_t m_roundNumber;
	uint8_t m_playerToDrawID;
	uint8_t m_ownerID;
	std::vector<Player> m_players;
	Turn m_turn;
	common::game::GameSettings m_gameSettings;
	Image m_image;
	Chat m_chat;
	common::game::GameState m_gameState;
};