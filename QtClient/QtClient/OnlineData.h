#pragma once

#include <cstdint>
#include <QString>

#include "common.h"

class OnlineData
{
public:
	OnlineData();
	OnlineData(uint64_t roomID, const QString& username);
	OnlineData(uint64_t&& roomID, QString&& username);
	
	OnlineData(const OnlineData&) = default;
	OnlineData& operator=(const OnlineData&) = default;

	OnlineData(OnlineData&&) = default;
	OnlineData& operator=(OnlineData&&) = default;

public:
	uint64_t GetRoomID() const;
	void SetRoomID(uint64_t);

	QString GetUsername() const;
	void SetUsername(const QString&);
	void SetUsername(QString&&);

	common::game::GameState GetGameState() const;
	void SetGameState(common::game::GameState);

private:
	uint64_t m_roomID;
	QString m_username;
	common::game::GameState m_gameState;
};