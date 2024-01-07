#include "OnlineData.h"
#include "macro.h"

#ifdef ONLINE
OnlineData::OnlineData() :
	m_roomID{},
	m_username{},
	m_gameState{ common::game::GameState::NONE }
{
	/* empty */
}

OnlineData::OnlineData(uint64_t roomID, const QString& username) :
	m_roomID{ roomID },
	m_username{ username },
	m_gameState{ common::game::GameState::NONE }
{
	/* empty */
}

OnlineData::OnlineData(uint64_t&& roomID, QString&& username) :
	m_roomID{ roomID },
	m_username{ username },
	m_gameState{ common::game::GameState::NONE }
{
	/* empty */
}

uint64_t OnlineData::GetRoomID() const
{
	return m_roomID;
}

void OnlineData::SetRoomID(uint64_t roomID)
{
	this->m_roomID = roomID;
}

QString OnlineData::GetUsername() const
{
	return m_username;
}

void OnlineData::SetUsername(const QString& username)
{
	this->m_username = username;
}

void OnlineData::SetUsername(QString&& username)
{
	this->m_username = username;
}

common::game::GameState OnlineData::GetGameState() const
{
	return m_gameState;
}

void OnlineData::SetGameState(common::game::GameState gameState)
{
	m_gameState = gameState;
}
#endif