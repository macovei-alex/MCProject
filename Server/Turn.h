#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <chrono>
#include <mutex>
#include <memory>

#include "Player.h"
#include "common.h"

namespace chr = std::chrono;

class Turn
{
public:
	Turn() noexcept = default;
	Turn(uint8_t turnNumber);
	Turn(const Turn& turn);
	Turn& operator=(const Turn& turn);
	~Turn() = default;
	uint8_t GetTurnNumber() const noexcept;
	chr::seconds GetTimer() const noexcept;
	std::string GetWord() const noexcept;
	void SetWord(const std::string& word) noexcept;
	void SetWord(std::string&& word) noexcept;
	void SetPlayersMutex(std::shared_ptr<std::mutex> playersMutex) noexcept;

public:
	void Reset(std::vector<Player>& players, size_t drawingPlayerID);
	void Reset(std::vector<Player>& players, Player& drawingPlayer);
	void Start(const std::vector<Player>& players, chr::seconds drawingTime, bool& m_stopped);

private:
	uint8_t m_turnNumber;
	std::string m_word;
	std::vector<uint8_t> m_showLetterIDs;
	std::chrono::system_clock::time_point m_playStartTime;
	std::shared_ptr<std::mutex> m_playersMutex;
};
