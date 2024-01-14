#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <mutex>
#include <memory>

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
	const Player& GetPlayer(const std::string& name);
	uint8_t GetRoundNumber() const;
	common::game::GameSettings GetGameSettings();
	void SetGameSettings(common::game::GameSettings gameSettings);
	const Turn& GetTurn() const;
	common::game::GameState GetGameState() const;
	void SetGameState(common::game::GameState gameState);
	std::mutex& GetPlayersMutex();
	bool IsRunning() const;
	void ChatEmplace(common::Message&&);
	void SetTurnWord(std::string&&);
	void SetTurnWord(const std::string&);
	void AppendImageUpdates(const std::vector<std::unordered_map<std::string, std::variant<int64_t, std::string>>>& jsonVector);
	crow::json::wvalue GetUpdatesJsonAfter(uint64_t timestamp);
	bool ChatEmpty();
	crow::json::wvalue GetMessagesOrderedJsonList(uint64_t start, const std::string& author);
	void SetPlayerGuessStatus(const std::string& username, bool status);

public:
	void Run();
	void RemovePlayer(const std::string& playerName);
	void RemoveDisconnectedPlayers();
	void Reset();
	void Stop();

	void AddPlayer(const Player& player);
	void AddPlayer(Player&& player);

private:
	uint8_t m_roundNumber;
	uint8_t m_ownerID;
	utils::ThreadSafe<std::vector<Player>> m_players;
	Turn m_turn;
	common::game::GameSettings m_gameSettings;
	utils::ThreadSafe<Image> m_image;
	utils::ThreadSafe<Chat> m_chat;
	std::atomic<common::game::GameState> m_gameState;
	bool m_stopped;
};