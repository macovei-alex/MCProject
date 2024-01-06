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
	auto result{ std::move(m_storage.get_all<db::Player>(
		sql::where(sql::c(&db::Player::playerName) == playerName))) };

	return result.size() == 1;
}

db::ReturnValue db::Database::SignUp(const std::string& playerName, const std::string& password)
{
	if (IfPlayerExist(playerName))
		return { false, "Player already exists" };

	db::Player player;

	player.playerName = playerName;
	player.password = std::move(utils::getHash(password));
	player.isOnline = true;

	m_storage.insert(player);

	return { true, "Account succesfully created" };
}

db::ReturnValue db::Database::SignIn(const std::string& playerName, const std::string& password)
{
	auto result{ std::move(m_storage.get_all<db::Player>(
		sql::where(sql::c(&db::Player::playerName) == playerName))) };

	if (result.empty())
		return{ false, "Player does not exist!" };

	if (result[0].isOnline)
		return{ false, "Player is already online!" };

	if (result[0].password != utils::getHash(password))
		return{ false, "Wrong password!" };

	result[0].isOnline = true;
	m_storage.update(result[0]);

	return { true, "Succesfully logged in" };
}

db::ReturnValue db::Database::SignOut(const std::string& playerName)
{
	auto result{ std::move(m_storage.get_all<db::Player>(
		sql::where(sql::c(&db::Player::playerName) == playerName))) };

	if (result.empty())
		return { false, "Player does not exist!" };

	result[0].isOnline = false;
	m_storage.update(result[0]);

	return { true, "Succesfully logged out" };
}

db::ReturnValue db::Database::AddGame(const std::string& playerName, int score, const std::string& difficulty, const std::string& date)
{
	auto player{ std::move(m_storage.get_all<db::Player>(
		sql::where(sql::c(&db::Player::playerName) == playerName))) };

	if (player.empty())
		return { false, std::format("No player with name {} has been found", playerName) };

	db::GameHistory game;
	game.playerID = player[0].id;
	game.score = score;
	game.difficulty = difficulty;
	game.date = date;

	m_storage.insert(game);

	return { true, "Game succesfully added" };
}

db::GameHistory db::Database::GetGameHistory(const std::string& playerName)
{
	auto players{ std::move(m_storage.get_all<db::Player>(
		sql::where(sql::c(&db::Player::playerName) == playerName))) };

	if (players.empty())
	{
		GameHistory ret;
		ret.playerID = -1;
		return ret;
	}

	auto& player{ players[0] };
	auto result{ std::move(m_storage.get_all<db::GameHistory>(
		sql::where(sql::c(&db::GameHistory::playerID) == player.id))) };

	size_t nr{ 0 };
	size_t numberOfGames{ result.size() };
	auto totalScore{ m_storage.sum<int>(&db::GameHistory::score,
		sql::where(sql::c(&db::GameHistory::playerID) == player.id)) };

	for (auto& game : result)
	{
		nr++;
		std::cout << nr << " " << game.score << " " << game.date << " " << game.difficulty << std::endl;
	}
	std::cout << "Total score: " << totalScore << '\n';
}

std::vector<std::string> db::Database::GetRandomWords(int number, const std::string& difficulty)
{
	std::vector<std::string> randomWords;
	randomWords.reserve(number);

	auto words{ std::move(m_storage.get_all<db::Word>(
		sql::where(sql::c(&db::Word::difficulty) == difficulty),
		sql::order_by(sql::random()),
		sql::limit(number))) };

	for (int i = 0; i < words.size(); i++)
	{
		randomWords.push_back(words[i].text);
	}

	return randomWords;
}
