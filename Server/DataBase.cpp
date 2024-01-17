#include "database.h"

#include <fstream>

#include "..\UtilsDLL\utilsDLL.h"

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
	std::string text;
	std::string difficulty;
	for (std::ifstream wordsIn{ "words.txt" }; !wordsIn.eof(); /* empty */)
	{
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
	auto players{ std::move(m_storage.get_all<db::Player>(
		sql::where(sql::c(&db::Player::username) == playerName))) };

	return players.size() == 1;
}

db::ReturnValue db::Database::SignUp(const std::string& username, const std::string& password)
{
	if (IfPlayerExist(username))
		return { false, "Player already exists" };

	db::Player player{ username, password };
	player.isOnline = true;

	m_storage.insert(player);

	return { true, "Account succesfully created" };
}

db::ReturnValue db::Database::SignIn(const std::string& username, const std::string& password)
{
	auto players{ std::move(m_storage.get_all<db::Player>(
		sql::where(sql::c(&db::Player::username) == username))) };

	if (players.empty())
		return{ false, "Player does not exist!" };

	if (players[0].isOnline)
		return{ false, "Player is already online!" };

	if (players[0].password != utils::GetHashSHA256(password))
		return{ false, "Wrong password!" };

	players[0].isOnline = true;
	m_storage.update(players[0]);

	return { true, "Succesfully logged in" };
}

db::ReturnValue db::Database::SignOut(const std::string& username)
{
	auto result{ std::move(m_storage.get_all<db::Player>(
		sql::where(sql::c(&db::Player::username) == username))) };

	if (result.empty())
		return { false, "Player does not exist!" };

	result[0].isOnline = false;
	m_storage.update(result[0]);

	return { true, "Succesfully logged out" };
}

db::ReturnValue db::Database::AddGame(const std::string& playerName, int score, const std::string& difficulty, const std::string& date)
{
	auto player{ std::move(m_storage.get_all<db::Player>(
		sql::where(sql::c(&db::Player::username) == playerName))) };

	if (player.empty())
		return { false, std::move(std::format("No player with name {} has been found", playerName)) };

	db::GameHistory game{ player[0].id, score, difficulty, date };

	m_storage.insert(game);

	return { true, "Game succesfully added" };
}

db::ReturnValueForHistory db::Database::GetGameHistory(const std::string& playerName)
{
	auto players{ std::move(m_storage.get_all<db::Player>(
		sql::where(sql::c(&db::Player::username) == playerName))) };

	std::vector<int> scores;
	std::vector<std::string> dates;
	std::vector<std::string> difficulties;

	if (players.empty())
	{
		GameHistory ret;
		ret.playerID = -1;
		return { scores, dates, difficulties, false, std::format("Player {} not found", playerName)};
	}

	auto& player{ players[0] };
	auto result{ std::move(m_storage.get_all<db::GameHistory>(
		sql::where(sql::c(&db::GameHistory::playerID) == player.id))) };

	size_t numberOfGames{ result.size() };
	auto totalScore{ m_storage.sum<int>(&db::GameHistory::score,
		sql::where(sql::c(&db::GameHistory::playerID) == player.id)) };

	std::ranges::for_each(result, [&scores, &dates, &difficulties](const db::GameHistory& game) {
		scores.push_back(game.score);
		dates.push_back(game.date);
		difficulties.push_back(game.difficulty);
		});

	return {scores, dates, difficulties, true, std::format("Found game history for player{}", playerName)};
}

std::vector<std::string> db::Database::GetRandomWords(uint64_t count, const std::string& difficulty)
{
	std::vector<std::string> randomWords;
	randomWords.reserve(count);

	auto words{ std::move(m_storage.get_all<db::Word>(
		sql::where(sql::c(&db::Word::difficulty) == difficulty),
		sql::order_by(sql::random()),
		sql::limit(count))) };

	std::ranges::for_each(words, [&randomWords](const db::Word& word) {
		randomWords.emplace_back(word.text);
		});

	return randomWords;
}

std::vector<std::string> db::Database::GetRandomWords(uint64_t count)
{
	std::vector<std::string> randomWords;
	randomWords.reserve(count);

	auto words{ std::move(m_storage.get_all<db::Word>(
		sql::order_by(sql::random()),
		sql::limit(count))) };

	std::ranges::for_each(words, [&randomWords](const db::Word& word) {
		randomWords.emplace_back(word.text);
		});

	return randomWords;
}

db::ReturnValue db::Database::ResetPlayerAccounts()
{
	auto players{ std::move(m_storage.get_all<db::Player>()) };

	std::ranges::for_each(players, [this](db::Player& player) {
		if (player.isOnline) {
			player.isOnline = false;
			m_storage.update(player);
		}
		});

	return { true, "All accounts have been reset" };
}
