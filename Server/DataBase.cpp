#include "database.h"
#include <fstream>

void Database::PopulateStorage()
{
	std::ifstream f("words.txt");
	std::vector<Word> words;
	int index = 0;
	while (!f.eof())
	{
		index++;
		std::string word;
		std::string difficulty;
		f >> word >> difficulty;
		words.push_back(Word(index, word, difficulty));
	}
	storage.insert_range(words.begin(), words.end());
}

bool Database::IfPlayerExist(const std::string& playerName)
{
	auto result = storage.get_all<PlayerDB>(
		sql::where(sql::c(&PlayerDB::playerName) == playerName)
	);
	return result.size() == 1;
}

bool Database::SignUp(const std::string& playerName, const std::string& password)
{
	try {
		if (IfPlayerExist(playerName))
		{
			throw "Player already exists!";
		}
		else
		{
			PlayerDB player;
			int lastID = storage.last_insert_rowid();
			player.playerName = playerName;
			player.password = password;
			player.isOnline = true;
			storage.insert(player);
			return true;
		}
	}
	catch (const char* msg)
	{
		std::cout<< msg << std::endl;
		return false;
	}
	
}

bool Database::SignIn(const std::string& playerName, const std::string& password)
{
	auto result = storage.get_all<PlayerDB>(
		sql::where(sql::c(&PlayerDB::playerName) == playerName)
	);
	bool IsPlayerOnline = result[0].isOnline;
	try {
		if (IsPlayerOnline)
		{
			throw "Player already online!";
		}
		if (result.size() == 0)
		{
			throw "Invalid username";
		}
		else
		{
			const std::string& playerPassword = result[0].password;
			if (playerPassword != password)
			{
				throw "Invalid password";
			}
			else
			{
				result[0].isOnline = true;
				storage.update(result[0]);
				return true;
			}
		}
	}
	catch (const char* msg)
	{
		std::cout << msg << std::endl;
		return false;
	}
}

bool Database::SignOut(const std::string& playerName)
{
	auto result = storage.get_all<PlayerDB>(
		sql::where(sql::c(&PlayerDB::playerName) == playerName)
	);
	result[0].isOnline = false;
	storage.update(result[0]);
	return true;
}

void Database::AddGame(const std::string& playerName, int score, const std::string& difficulty, const std::string& date)
{
	auto player = storage.get_all<PlayerDB>(
		sql::where(sql::c(&PlayerDB::playerName) == playerName)
	);
	int playerID = player[0].id;
	GameHistory game;
	game.playerID = playerID;
	game.score = score;
	game.difficulty = difficulty;
	game.date = date;
	storage.insert(game);
}

void Database::GetGameHistory(const std::string& playerName)
{
	auto player = storage.get_all<PlayerDB>(
		sql::where(sql::c(&PlayerDB::playerName) == playerName)
	);
	int playerID = player[0].id;
	auto result = storage.get_all<GameHistory>(
		sql::where(sql::c(&GameHistory::playerID) == playerID)
	);
	int nr = 0;
	int numberOfGames = result.size();
	auto totalScore = storage.sum<int>(&GameHistory::score, sql::where(sql::c(&GameHistory::playerID) == playerID));
	for (auto& game : result)
	{
		nr++;
		std::cout << nr << " " << game.score << " " << game.date << " " << game.difficulty << std::endl;
	}
	std::cout << "Total score: " << totalScore << std::endl;
}

std::vector<std::string> Database::GetRandomWords(int number, const std::string& difficulty)
{
	std::vector<std::string> randomWords;
	auto words = storage.get_all<Word>(
		sql::where(sql::c(&Word::difficulty) == difficulty),
		sql::order_by(sql::random()),
		sql::limit(number)
	);
	for(int i = 0; i < words.size(); i++)
	{
		randomWords.push_back(words[i].text);
	}
	return randomWords;
}
