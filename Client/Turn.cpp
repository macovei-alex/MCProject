#include "Turn.h"

const std::string Turn::s_wordsFilePath = "words.in";

Turn::Turn(uint8_t turnNumber) :
	m_turnNumber{ turnNumber },
	m_word{},
	m_showLetterIDs{}
{
	m_chooseWordStartTime = clock();
	m_playStartTime = std::numeric_limits<double>::max();
	m_fin = std::ifstream(s_wordsFilePath);
}

Turn::Turn(const Turn& other) :
	m_turnNumber{ other.m_turnNumber },
	m_word{ other.m_word },
	m_showLetterIDs{ other.m_showLetterIDs },
	m_playStartTime{ other.m_playStartTime },
	m_chooseWordStartTime{ other.m_chooseWordStartTime },
	m_fin{ s_wordsFilePath }
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

	m_fin = std::ifstream(s_wordsFilePath);
	// m_fin.seekg(turn.m_fin.tellg());

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

float_t Turn::GetChoiceTime() const
{
	return float_t(clock() - m_chooseWordStartTime) / CLOCKS_PER_SEC;
}

float_t Turn::GetPlayTime() const
{
	return float_t(clock() - m_playStartTime) / CLOCKS_PER_SEC;
}

std::string Turn::GetWord() const
{
	return m_word;
}

std::vector<std::string> Turn::generateWordChoices(const uint8_t wordsCount)
{
	std::vector<std::string> words(wordsCount);
	for (uint8_t i = 0; i < wordsCount && !m_fin.eof(); i++)
	{
		m_fin >> words[i];
	}
	return words;
}

void Turn::sendWordChoices(const Player& player, const std::vector<std::string>& wordChoices) const
{
	// [send] wordChoices [to] player
}

void Turn::choosingWordPhase(const std::vector<Player>& players)
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

void Turn::startNewTurn(std::vector<Player>& players)
{
	choosingWordPhase(players);
	for (auto& player : players)
	{
		// player.resetCurrentScore();
	}
	m_playStartTime = clock();
}

void Turn::endTurn(std::vector<Player>& players)
{
	for (Player& player : players)
	{
		player.addScore();
	}
}

void Turn::startNextTurn(std::vector<Player>& players)
{
	endTurn(players);
	m_turnNumber++;
	startNewTurn(players);
}