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
#include "..\UtilsDLL\utilsDLL.h"

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
	void SetTurnNumber(uint8_t) noexcept;
	chr::seconds GetTimer() const noexcept;
	std::string GetWord() const noexcept;
	void SetWord(const std::string& word) noexcept;
	void SetWord(std::string&& word) noexcept;

public:
	void Reset(utils::ThreadSafe<std::vector<Player>>& players, size_t drawingPlayerID);
	void Reset(utils::ThreadSafe<std::vector<Player>>& players, Player& drawingPlayer);
	void Start(const utils::ThreadSafe<std::vector<Player>>& players, chr::seconds drawingTime, bool& m_stopped);

private:
	std::atomic<uint8_t> m_turnNumber;
	utils::ThreadSafe<std::string> m_word;
	utils::ThreadSafe<std::vector<uint8_t>> m_showLetterIDs;
	utils::ThreadSafe<std::chrono::system_clock::time_point> m_playStartTime;
};
