#include "OnlineData.h"
#include "macro.h"

#ifdef ONLINE
OnlineData::OnlineData() :
	m_roomID{},
	m_username{},
	m_gameState{ common::game::GameState::NONE },
	m_role{ common::game::PlayerRole::NONE }
{
	/* empty */
}

OnlineData::OnlineData(uint64_t roomID, const QString& username) :
	m_roomID{ roomID },
	m_username{ username },
	m_gameState{ common::game::GameState::NONE },
	m_role{ common::game::PlayerRole::NONE }
{
	/* empty */
}

OnlineData::OnlineData(uint64_t roomID, QString&& username) :
	m_roomID{ roomID },
	m_username{ username },
	m_gameState{ common::game::GameState::NONE },
	m_role{ common::game::PlayerRole::NONE }
{
	/* empty */
}
#endif