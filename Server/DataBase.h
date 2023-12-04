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
		int id;
		std::string playerID;//foreign key
		int score;
		std::string difficulty;
		std::string date;
	};

	inline auto CreateStorage(const std::string& filename)
	{
		return sql::make_storage(filename,
			sql::make_table("word",
				sql::make_column("id", &Word::id, sql::primary_key().autoincrement()),
				sql::make_column("word", &Word::text),
				sql::make_column("difficulty", &Word::difficulty)),
			sql::make_table("player",
				sql::make_column("id", &PlayerDB::id, sql::primary_key().autoincrement()),
				sql::make_column("playerName", &PlayerDB::playerName),
				sql::make_column("password", &PlayerDB::password),
				sql::make_column("isOnline", &PlayerDB::isOnline)),
			sql::make_table("gameHistory",
				sql::make_column("id", &GameHistory::id, sql::primary_key().autoincrement()),
				sql::make_column("playerID", &GameHistory::playerID),
				sql::foreign_key(&GameHistory::playerID).references(&PlayerDB::id),
				sql::make_column("score", &GameHistory::score),
				sql::make_column("difficulty", &GameHistory::difficulty),
				sql::make_column("date", &GameHistory::date))
		);
	}
	void PopulateStorage();
	//void SignInOrUp(const std::string& playerName, const std::string& password);
	bool IsPlayerExist(const std::string& playerName);
	void SignUp(const std::string& playerName, const std::string& password);
	void SignIn(const std::string& playerName, const std::string& password);
	void SignOut(const std::string& playerName);
	void AddGame(const std::string& playerName, int score, const std::string& difficulty, const std::string& date);
	void GetGameHistory(const std::string& playerName);
	void GetRandomWords(int number ,const std::string& difficulty);
};