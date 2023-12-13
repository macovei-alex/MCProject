#pragma once

#include <cstdint>

class GameSettings
{
public:
	GameSettings();
	GameSettings(uint8_t roundCount, uint8_t drawTime, uint8_t chooseWordOtionCount);
	GameSettings(const GameSettings& other) noexcept = default;
	GameSettings& operator=(const GameSettings& other) noexcept = default;
	GameSettings(GameSettings&& other) noexcept = default;
	GameSettings& operator=(GameSettings&& other) noexcept = default;
	~GameSettings() noexcept = default;

public:
	uint16_t GetRoundCount() const noexcept;
	uint16_t GetDrawTime() const noexcept;
	uint16_t GetChooseWordOptionCount() const noexcept;

public:
	void SetRoundCount(uint16_t roundCount) noexcept;
	void SetDrawTime(uint16_t drawTime) noexcept;
	void SetChooseWordOptionCount(uint16_t chooseWordOptionCount) noexcept;

private:
	static const uint16_t defaultRoundCount;
	static const uint16_t defaultDrawTime;
	static const uint16_t defaultChooseWordOptionCount;

	uint16_t m_roundCount;
	uint16_t m_drawTime;
	uint16_t m_chooseWordOptionCount;
};

