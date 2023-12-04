#include "database.h"
#include <fstream>

void Database::PopulateStorage()
{
	auto storage = CreateStorage("");
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

bool Database::IsPlayerExist(const std::string& playerName)
{
	auto storage = CreateStorage("");
	auto result = storage.get_all<PlayerDB>(
		sql::where(sql::c(&PlayerDB::playerName) == playerName)
	);
	return result.size() == 1;
}

void Database::SignUp(const std::string& playerName, const std::string& password)
{
	if(IsPlayerExist(playerName))
	{
		throw std::exception("Player already exists!");
	}
	auto storage = CreateStorage("");
	PlayerDB player;
	int lastID = storage.last_insert_rowid();
	player.playerName = playerName;
	player.password = password;
	player.isOnline = true;
	storage.insert(player);
}

void Database::SignIn(const std::string& playerName, const std::string& password)
{
	auto storage = CreateStorage("");
	auto result = storage.get_all<PlayerDB>(
		sql::where(sql::c(&PlayerDB::playerName) == playerName)
	);
	bool IsPlayerOnline = result[0].isOnline;
	if (IsPlayerOnline)
	{
		throw std::exception("Player already online!");
	}
	if (result.size() == 0)
	{
		throw std::exception("Invalid username");
	}
	else
	{
		const std::string& playerPassword = result[0].password;
		if (playerPassword != password)
		{
			throw std::exception("Invalid password");
		}
		else
		{
			result[0].isOnline = true;
			storage.update(result[0]);
		}
	}
}

void Database::SignOut(const std::string& playerName)
{
	auto storage = CreateStorage("");
	auto result = storage.get_all<PlayerDB>(
		sql::where(sql::c(&PlayerDB::playerName) == playerName)
	);
	result[0].isOnline = false;
	storage.update(result[0]);
}
