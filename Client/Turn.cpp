#include "Turn.h"
#include <iostream>

Turn::Turn(uint8_t turnNumber, const std::string& word, int8_t turnToDraw) :
	m_turnNumber{ turnNumber },
	m_word{ word },
	m_showLetterIDs{},
	m_turnToDraw{ turnToDraw }
{
	m_startTime = clock();
}

Turn::Turn(const Turn& turn) :
	m_turnNumber{ turn.m_turnNumber },
	m_word{ turn.m_word },
	m_showLetterIDs{ turn.m_showLetterIDs },
	m_turnToDraw{ turn.m_turnToDraw },
	m_startTime{ turn.m_startTime }
{
	/* empty */
}

Turn& Turn::operator=(const Turn& turn)
{
	m_turnNumber = turn.m_turnNumber;
	m_word = turn.m_word;
	m_showLetterIDs = turn.m_showLetterIDs;
	m_turnToDraw = turn.m_turnToDraw;
	m_startTime = turn.m_startTime;
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

double_t Turn::GetTime() const
{
	return double_t(clock() - m_startTime) / CLOCKS_PER_SEC;
}

std::string Turn::GetWord() const
{
	return m_word;
}

std::string Turn::generateWord() const
{
	return "";
}

void Turn::startNewTurn(const std::vector<Player>& players)
{
	m_word = generateWord();
	for (auto& player : players)
	{
		//player.resetCurrentScore();
	}
	m_startTime = clock();
}

void Turn::endTurn(const std::vector<Player>& players)
{
	for (auto& player : players)
	{
		//players.addScore(players.GetCurrentScore());
	}
}