#pragma once

#include "Player.h"
#include "Turn.h"
#include "../Common/GameSettings.h"
#include "Image.h"
#include "Chat.h"

#include <vector>
#include <cstdint>
#include <string>

class Game
{
public:
	Game() noexcept = default;
	Game(const Game& other) noexcept = default;
	Game& operator=(const Game& other) noexcept = default;
	Game(Game&& other) noexcept;
	Game& operator=(Game&& other) noexcept;
	~Game() noexcept = default;

public:
	const std::vector<Player>& GetPlayers();
	uint8_t GetRoundNumber();
	void SetRoundNumber(uint8_t roundNumber);
	uint8_t GetPlayerToDrawID();
	void SetPlayerToDrawID(uint8_t playerToDrawID);
	GameSettings& GetGameSettings();
	Turn& GetTurn();
	Image& GetImage();
	Chat& GetChat();

public:
	void Start();
	void AddPlayer(const Player& player);
	void RemovePlayer(const std::string& playerName);

private:
	uint8_t m_roundNumber;
	uint8_t m_playerToDrawID;
	uint8_t m_ownerID;
	std::vector<Player> m_players;
	Turn m_turn;
	GameSettings m_gameSettings;
	Image m_image;
	Chat m_chat;
};