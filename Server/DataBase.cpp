#include "database.h"

#include <fstream>

db::Database::Database(const std::string& filename) :
	m_storage{ db::CreateStorage(filename) }
{
	m_storage.sync_schema();
	if (m_storage.count<db::Word>() == 0)
		PopulateStorage();

	/*std::vector<db::Word> words{ std::move(m_storage.get_all<db::Word>()) };
	for (const auto& word : words)
		std::cout << std::format("{}. {} {}\n", word.id, word.text, word.difficulty);*/
}

void db::Database::PopulateStorage()
{
	std::vector<db::Word> words;
	for (std::ifstream wordsIn{ "words.txt" }; !wordsIn.eof(); /* empty */)
	{
		std::string text;
		std::string difficulty;
		wordsIn >> text >> difficulty;
		words.push_back(db::Word{ std::move(text), std::move(difficulty) });
	}

	m_storage.insert_range(words.begin(), words.end());

	// The first word is being memorized wrongly, for some unknown reason. 
	// When we first initialize the database, we have to make sure the first word starts with a letter.

	db::Word firstWord{ std::move(m_storage.get<db::Word>(1)) };
	for (uint8_t c{ (uint8_t)firstWord.text[0] }; !std::isalpha(c); c = firstWord.text.erase(0, 1)[0]);
	m_storage.update(firstWord);
}

bool db::Database::IfPlayerExist(const std::string& playerName)
{
	auto result = m_storage.get_all<db::Player>(
		sql::where(sql::c(&db::Player::playerName) == playerName)
	);
	return result.size() == 1;
}

db::ReturnValue db::Database::SignUp(const std::string& playerName, const std::string& password)
{
	try
	{
		if (IfPlayerExist(playerName))
		{
			throw std::exception("Player already exists!");
		}
		else
		{
			db::Player player;
			int lastID = m_storage.last_insert_rowid();
			player.playerName = playerName;
			player.password = password;
			player.isOnline = true;
			m_storage.insert(player);

			return {true, "Account succesfully created" };
		}
	}
	catch (const std::exception& exception)
	{
		/*Logger logger(std::cout);
		logger.Log(errorMessage, Logger::Level::Error);*/
		return {false, exception.what() };
	}

}

db::ReturnValue db::Database::SignIn(const std::string& playerName, const std::string& password)
{
	auto result = m_storage.get_all<db::Player>(
		sql::where(sql::c(&db::Player::playerName) == playerName)
	);
	bool playerOnline = result[0].isOnline;
	std::string errorMessage;
	try {
		if (result.size() == 0)
		{
			throw std::exception("Player does not exist!");
		}
		if (playerOnline)
		{
			throw std::exception("Player is already online!");
		}
		else
		{
			const std::string& playerPassword = result[0].password;
			if (playerPassword != password)
			{
				throw std::exception("Wrong password!");
			}
			else
			{
				result[0].isOnline = true;
				m_storage.update(result[0]);
				return {true, "Succesfully logged in" };
			}
		}
	}
	catch (const std::exception& exception)
	{
		return {false, exception.what() };
	}
}

db::ReturnValue db::Database::SignOut(const std::string& playerName)
{
	auto result = m_storage.get_all<db::Player>(
		sql::where(sql::c(&db::Player::playerName) == playerName)
	);
	result[0].isOnline = false;
	m_storage.update(result[0]);

	return {true, "Succesfully logged out" };
}

void db::Database::AddGame(const std::string& playerName, int score, const std::string& difficulty, const std::string& date)
{
	auto player = m_storage.get_all<db::Player>(
		sql::where(sql::c(&db::Player::playerName) == playerName)
	);
	int playerID = player[0].id;
	db::GameHistory game;
	game.playerID = playerID;
	game.score = score;
	game.difficulty = difficulty;
	game.date = date;
	m_storage.insert(game);
}

void db::Database::GetGameHistory(const std::string& playerName)
{
	auto player = m_storage.get_all<db::Player>(
		sql::where(sql::c(&db::Player::playerName) == playerName)
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

std::vector<std::string> db::Database::GetRandomWords(int number, const std::string& difficulty)
{
	std::vector<std::string> randomWords;
	auto words = m_storage.get_all<db::Word>(
		sql::where(sql::c(&db::Word::difficulty) == difficulty),
		sql::order_by(sql::random()),
		sql::limit(number)
	);
	for (int i = 0; i < words.size(); i++)
	{
		randomWords.push_back(words[i].text);
	}
	return randomWords;
}
