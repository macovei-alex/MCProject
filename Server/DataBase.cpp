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

void Database::SignUp(const std::string& playerName, const std::string& password)
{
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
		// , sql::where(sql::c(&PlayerDB::password) == password) Nu functioneaza asa, da eroare de compilare. Trebuie probabil facut un AND intre conditii
	);
	if (result.size() == 1)
	{
		storage.update_all(sql::set(sql::c(&PlayerDB::isOnline) = true),
						sql::where(sql::c(&PlayerDB::playerName) == playerName)
						//, sql::where(sql::c(&PlayerDB::password) == password) Nu functioneaza asa, da eroare de compilare. Trebuie probabil facut un AND intre conditii
				);
	}
	else
	{
		throw std::exception("Invalid username or password!");
	}
}
