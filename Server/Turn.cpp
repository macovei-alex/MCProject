#include "Turn.h"

Turn::Turn(uint8_t turnNumber) :
	m_turnNumber{ turnNumber },
	m_word{},
	m_showLetterIDs{}
{
	m_chooseWordStartTime = clock();
	m_playStartTime = std::numeric_limits<double>::max();
}

Turn::Turn(const Turn& other) :
	m_turnNumber{ other.m_turnNumber },
	m_word{ other.m_word },
	m_showLetterIDs{ other.m_showLetterIDs },
	m_playStartTime{ other.m_playStartTime },
	m_chooseWordStartTime{ other.m_chooseWordStartTime }
{
	// m_fin.seekg(turn.m_fin.tellg());
}

Turn& Turn::operator=(const Turn& other)
{
	if (this == &other)
		return *this;
	m_turnNumber = other.m_turnNumber;
	m_word = other.m_word;
	m_showLetterIDs = other.m_showLetterIDs;
	m_playStartTime = other.m_playStartTime;
	m_chooseWordStartTime = other.m_chooseWordStartTime;

	return *this;
}

Turn::~Turn()
{
	/* EMPTY */
}

uint8_t Turn::GetTurnNumber() const noexcept
{
	return m_turnNumber;
}

float_t Turn::GetChoiceTime() const noexcept
{
	return float_t(clock() - m_chooseWordStartTime) / CLOCKS_PER_SEC;
}

float_t Turn::GetPlayTime() const noexcept
{
	return float_t(clock() - m_playStartTime) / CLOCKS_PER_SEC;
}

std::string Turn::GetWord() const noexcept
{
	return m_word;
}

void Turn::ChoosingWordPhase(const std::vector<Player>& players)
{
	//std::vector<std::string> wordChoices = generateWordChoices(GameSettings::chooseWordOptionCount);
	//int i;
	//for (i = 0; i < players.size(); i++)
	//	if (players[i].getRole() == Player::Role::Draw)
	//	{
	//		sendWordChoices(players[i], wordChoices);
	//		break;
	//	}
	//
	//while (GetChooseTime() < GameSettings::chooseWordTime)
	//	// [check] receivedWordChoice [from] player
}

void Turn::StartNewTurn(std::vector<Player>& players)
{
	ChoosingWordPhase(players);
	for (auto& player : players)
	{
		// player.resetCurrentScore();
	}
	m_playStartTime = clock();
}

void Turn::EndTurn(std::vector<Player>& players)
{
	for (Player& player : players)
	{
		player.addScore();
	}
}

void Turn::StartNextTurn(std::vector<Player>& players)
{
	EndTurn(players);
	m_turnNumber++;
	StartNewTurn(players);
}