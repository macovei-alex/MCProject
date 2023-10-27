#pragma once
#include <cstdint>
#include <string>

class Player
{
public:
	Player(const std::string& m_name);
	Player(const Player& player);

private:
	std::string m_name;
	uint8_t m_score;
	bool m_flagGuessedCorrrectWord;
};

