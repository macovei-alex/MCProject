#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>

namespace sql = sqlite_orm;

class Database
{
public:
	struct Word
	{
		int id;
		std::string text;
		std::string difficulty;
	};

	struct PlayerDB
	{
		int id;//primary key
		std::string playerName;
		std::string password;
		bool isOnline;
	};

	struct GameHistory
	{
		int id;//primary key
		int playerID;//foreign key
		int score;
		std::string difficulty;
		std::string date;
	};

	//Database(const std::string& filename);
	void PopulateStorage();
	//void SignInOrUp(const std::string& playerName, const std::string& password);
	bool IfPlayerExist(const std::string& playerName);
	bool SignUp(const std::string& playerName, const std::string& password);
	bool SignIn(const std::string& playerName, const std::string& password);
	bool SignOut(const std::string& playerName);
	void AddGame(const std::string& playerName, int score, const std::string& difficulty, const std::string& date);
	void GetGameHistory(const std::string& playerName);
	void GetRandomWords(int number, const std::string& difficulty);

private:
	decltype(CreateStorage("")) Storage;
};

inline auto CreateStorage(const std::string& filename)
{
	return sql::make_storage(filename,
		sql::make_table("word",
			sql::make_column("id", &Database::Word::id, sql::primary_key().autoincrement()),
			sql::make_column("word", &Database::Word::text),
			sql::make_column("difficulty", &Database::Word::difficulty)),
		sql::make_table("player",
			sql::make_column("id", &Database::PlayerDB::id, sql::primary_key().autoincrement()),
			sql::make_column("playerName", &Database::Database::PlayerDB::playerName),
			sql::make_column("password", &Database::PlayerDB::password),
			sql::make_column("isOnline", &Database::PlayerDB::isOnline)),
		sql::make_table("gameHistory",
			sql::make_column("id", &Database::GameHistory::id, sql::primary_key().autoincrement()),
			sql::make_column("playerID", &Database::GameHistory::playerID),
			sql::foreign_key(&Database::GameHistory::playerID).references(&Database::PlayerDB::id),
			sql::make_column("score", &Database::GameHistory::score),
			sql::make_column("difficulty", &Database::GameHistory::difficulty),
			sql::make_column("date", &Database::GameHistory::date))
	);
}