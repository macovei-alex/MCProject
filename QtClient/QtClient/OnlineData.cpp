#include "OnlineData.h"
#include "macro.h"

#ifdef ONLINE
OnlineData::OnlineData() :
	roomID{},
	timeLeft{ INT_MAX },
	username{},
	chosenWord{},
	gameState{ common::game::GameState::NONE },
	playerRole{ common::game::PlayerRole::NONE }
{
	/* empty */
}

OnlineData::OnlineData(uint64_t roomID, const QString& username) :
	roomID{ roomID },
	timeLeft{ INT_MAX },
	username{ username },
	chosenWord{},
	gameState{ common::game::GameState::NONE },
	playerRole{ common::game::PlayerRole::NONE }
{
	/* empty */
}

OnlineData::OnlineData(uint64_t roomID, QString&& username) :
	roomID{ roomID },
	timeLeft{ INT_MAX },
	username{ username },
	chosenWord{},
	gameState{ common::game::GameState::NONE },
	playerRole{ common::game::PlayerRole::NONE }
{
	/* empty */
}
#endif