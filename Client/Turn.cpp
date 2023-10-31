#include "Turn.h"
#include <iostream>
#include <limits>

const std::string Turn::wordsFilePath = "words.in";

Turn::Turn(uint8_t turnNumber, const std::string& word, int8_t turnToDraw) :
	m_turnNumber{ turnNumber },
	m_word{ word },
	m_showLetterIDs{},
	m_turnToDraw{ turnToDraw }
{
	m_chooseWordStartTime = clock();
	m_startTime = std::numeric_limits<double>::max();
	m_fin = std::ifstream(wordsFilePath);
}

Turn::Turn(Turn& turn) :
	m_turnNumber{ turn.m_turnNumber },
	m_word{ turn.m_word },
	m_showLetterIDs{ turn.m_showLetterIDs },
	m_turnToDraw{ turn.m_turnToDraw },
	m_startTime{ turn.m_startTime },
	m_chooseWordStartTime{ turn.m_chooseWordStartTime },
	m_fin{ wordsFilePath }
{
	m_fin.seekg(turn.m_fin.tellg());
}

Turn& Turn::operator=(Turn& turn)
{
	m_turnNumber = turn.m_turnNumber;
	m_word = turn.m_word;
	m_showLetterIDs = turn.m_showLetterIDs;
	m_turnToDraw = turn.m_turnToDraw;
	m_startTime = turn.m_startTime;
	m_chooseWordStartTime = turn.m_chooseWordStartTime;

	m_fin = std::ifstream(wordsFilePath);
	m_fin.seekg(turn.m_fin.tellg());

	return *this;
}

Turn::~Turn()
{
	m_fin.close();
}

uint8_t Turn::GetTurnNumber() const
{
	return m_turnNumber;
}

double_t Turn::GetChooseTime() const
{
	return double_t(clock() - m_chooseWordStartTime) / CLOCKS_PER_SEC;
}

double_t Turn::GetPlayTime() const
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

void Turn::startNewTurn(std::vector<Player>& players)
{
	m_word = generateWord();
	for (auto& player : players)
	{
		//player.resetCurrentScore();
	}
	m_startTime = clock();
}

void Turn::endTurn(std::vector<Player>& players)
{
	for (Player& player : players)
	{
		player.addScore();
	}
}