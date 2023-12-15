#include "database.h"
#include <fstream>

Database::Database(const std::string& filename) :
	m_storage{ db::CreateStorage(filename) }
{
	m_storage.sync_schema();
	if(m_storage.count<db::Word>() == 0)
		PopulateStorage();
}

void Database::PopulateStorage()
{
	std::ifstream f("words.txt");
	std::vector<db::Word> words;
	int index = 0;
	while (!f.eof())
	{
		index++;
		std::string word;
		std::string difficulty;
		f >> word >> difficulty;
		words.push_back(db::Word(index, word, difficulty));
	}
	m_storage.insert_range(words.begin(), words.end());
}

bool Database::IfPlayerExist(const std::string& playerName)
{
	auto result = m_storage.get_all<db::PlayerDB>(
		sql::where(sql::c(&db::PlayerDB::playerName) == playerName)
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
			db::PlayerDB player;
			int lastID = m_storage.last_insert_rowid();
			player.playerName = playerName;
			player.password = password;
			player.isOnline = true;
			m_storage.insert(player);
			return true;
		}
	}
	catch (const char* msg)
	{
		std::cout << msg << std::endl;
		return false;
	}
	
}

bool Database::SignIn(const std::string& playerName, const std::string& password)
{
	auto result = m_storage.get_all<db::PlayerDB>(
		sql::where(sql::c(&db::PlayerDB::playerName) == playerName)
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
				m_storage.update(result[0]);
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
	auto result = m_storage.get_all<db::PlayerDB>(
		sql::where(sql::c(&db::PlayerDB::playerName) == playerName)
	);
	result[0].isOnline = false;
	m_storage.update(result[0]);
	return true;
}

void Database::AddGame(const std::string& playerName, int score, const std::string& difficulty, const std::string& date)
{
	auto player = m_storage.get_all<db::PlayerDB>(
		sql::where(sql::c(&db::PlayerDB::playerName) == playerName)
	);
	int playerID = player[0].id;
	db::GameHistory game;
	game.playerID = playerID;
	game.score = score;
	game.difficulty = difficulty;
	game.date = date;
	m_storage.insert(game);
}

void Database::GetGameHistory(const std::string& playerName)
{
	auto player = m_storage.get_all<db::PlayerDB>(
		sql::where(sql::c(&db::PlayerDB::playerName) == playerName)
	);
	int playerID = player[0].id;
	auto result = m_storage.get_all<db::GameHistory>(
		sql::where(sql::c(&db::GameHistory::playerID) == playerID)
	);
	int nr = 0;
	int numberOfGames = result.size();
	auto totalScore = m_storage.sum<int>(&db::GameHistory::score, sql::where(sql::c(&db::GameHistory::playerID) == playerID));
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
	auto words = m_storage.get_all<db::Word>(
		sql::where(sql::c(&db::Word::difficulty) == difficulty),
		sql::order_by(sql::random()),
		sql::limit(number)
	);
	for(int i = 0; i < words.size(); i++)
	{
		randomWords.push_back(words[i].text);
	}
	return randomWords;
}
