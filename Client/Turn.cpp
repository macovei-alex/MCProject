#include "Turn.h"

Turn::Turn(uint8_t turnNo, const std::string& word, uint8_t turnToDraw) : 
	m_turnNo(turnNo),
	m_word(word),
	m_showLetterIDs({}),
	m_turnToDraw(turnToDraw)
	//, m_timer()
{ /* empty */ }

Turn::Turn(const Turn& turn) :
	m_turnNo(turn.m_turnNo),
	m_word(turn.m_word),
	m_showLetterIDs(turn.m_showLetterIDs),
	m_turnToDraw(turn.m_turnToDraw)
{ /* empty */ }

Turn& Turn::operator=(const Turn& turn)
{
	m_turnNo = turn.m_turnNo;
	m_word = turn.m_word;
	m_showLetterIDs = turn.m_showLetterIDs;
	m_turnToDraw = turn.m_turnToDraw;
	return *this;
}

Turn::~Turn()
{
	/* empty */
}