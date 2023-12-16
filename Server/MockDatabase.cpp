#include "MockDatabase.h"

MockDatabase::MockDatabase(const std::string& filename)
{
	/* mock empty */
}

void MockDatabase::PopulateStorage()
{
	/* mock empty */
}

bool MockDatabase::IfPlayerExist(const std::string& playerName)
{
	return false;
}

db::ReturnValue MockDatabase::SignUp(const std::string& playerName, const std::string& password)
{
	return { true, "" };
}

db::ReturnValue MockDatabase::SignIn(const std::string& playerName, const std::string& password)
{
	return { true, "" };
}

db::ReturnValue MockDatabase::SignOut(const std::string& playerName)
{
	return { true, "" };
}

void MockDatabase::AddGame(const std::string& playerName, int score, const std::string& difficulty, const std::string& date)
{
	/* mock empty */
}

void MockDatabase::GetGameHistory(const std::string& playerName)
{
	/* mock empty */
}

std::vector<std::string> MockDatabase::GetRandomWords(int number, const std::string& difficulty)
{
	static const std::map<std::string, std::vector<std::string>> mockWords = {
		{"easy", {"mock1", "mock2", "mock3", "mock4"}},
		{"medium", {"mock5", "mock6", "mock7", "mock8"}},
		{"hard", {"mock9", "mock10", "mock11", "mock12"}}
	};

	std::vector<std::string> words;
	for (size_t i = 0; i < number; i++)
		words.push_back(mockWords.at(difficulty)[i]);
	return words;
}
