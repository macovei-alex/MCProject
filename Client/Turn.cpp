#include "Turn.h"
#include <iostream>

Turn::Turn(uint8_t turnNumber, const std::string& word, int8_t turnToDraw) :
	m_turnNumber{ turnNumber },
	m_word{ word },
	m_showLetterIDs{},
	m_turnToDraw{ turnToDraw },
	m_timer{ clock() }
{ /* empty */ }

Turn::Turn(const Turn& turn) :
	m_turnNumber{ turn.m_turnNumber },
	m_word{ turn.m_word },
	m_showLetterIDs{ turn.m_showLetterIDs },
	m_turnToDraw{ turn.m_turnToDraw },
	m_timer{ turn.m_timer }
{ /* empty */
}

Turn& Turn::operator=(const Turn& turn)
{
	m_turnNumber = turn.m_turnNumber;
	m_word = turn.m_word;
	m_showLetterIDs = turn.m_showLetterIDs;
	m_turnToDraw = turn.m_turnToDraw;
	return *this;
}

Turn::~Turn()
{
	/* empty */
}

uint8_t Turn::GetTurnNumber() const
{
	return m_turnNumber;
}

clock_t Turn::GetTime() const
{
	return m_timer;
}

//std::string Turn::GetWord()
//{
//	return m_word;
//}
//
//std::string Turn::generateWord()
//{
//	return "";
//}
//
//void Turn::startNewTurn(const std::vector<Player>& players)
//{
//	m_word = generateWord();
//	for (auto& player : players)
//	{
//		player.resetCurrentScore();
//	}
//}
//
//void Turn::endTurn(const std::vector<Player>& players)
//{
//	for (auto& player : players)
//	{
//		players.addScore(players.GetCurrentScore());
//	}
//}