#pragma once

#include <cstdint>
#include <QString>

#include "common.h"

struct OnlineData
{
	uint64_t m_roomID;
	QString m_username;
	common::game::GameState m_gameState;
	common::game::PlayerRole m_role;

	OnlineData();
	OnlineData(uint64_t roomID, const QString& username);
	OnlineData(uint64_t roomID, QString&& username);

	OnlineData(const OnlineData&) = default;
	OnlineData& operator=(const OnlineData&) = default;

	OnlineData(OnlineData&&) = default;
	OnlineData& operator=(OnlineData&&) = default;
};