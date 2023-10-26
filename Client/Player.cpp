#include "Player.h"

Player::Player(const std::string& name):
	m_name(name),
	m_score(0),
	m_flagGuessedCorrrectWord(false)
{
	/*EMPTY*/
}

Player::Player(const Player& player) :
	m_name(player.m_name),
	m_score(player.m_score),
	m_flagGuessedCorrrectWord(player.m_flagGuessedCorrrectWord)
{
	/*EMPTY*/
}
