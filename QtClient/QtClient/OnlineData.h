#pragma once
#include "macro.h"

#include <cstdint>
#include <QString>

#include "common.h"

struct OnlineData
{
	uint64_t roomID;
	uint64_t timeLeft;
	QString username;
	QString chosenWord;
	common::game::GameState gameState;
	common::game::PlayerRole playerRole;

	OnlineData();
	OnlineData(uint64_t roomID, const QString& username);
	OnlineData(uint64_t roomID, QString&& username);

	OnlineData(const OnlineData&) = default;
	OnlineData& operator=(const OnlineData&) = default;

	OnlineData(OnlineData&&) = default;
	OnlineData& operator=(OnlineData&&) = default;
};