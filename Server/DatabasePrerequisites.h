#pragma once

#include <string>
#include <sqlite_orm/sqlite_orm.h>

namespace sql = sqlite_orm;

namespace db
{
	struct ReturnValue
	{
		bool success;
		std::string_view reason;

		ReturnValue() = default;
		ReturnValue(bool success, const std::string_view& reason)
			: success{ success }, reason{ reason }
		{ /* empty */ }
		ReturnValue(bool success, std::string_view&& reason)
			: success{ success }, reason{ reason }
		{ /* empty */ }
	};

	struct ReturnValueForHistory
	{
		std::vector<int> scores;
		std::vector<std::string> dates;
		std::vector<std::string> difficulties;
		bool success;
		std::string reason;
	};

	struct Word
	{
		int id;
		std::string text;
		std::string difficulty;
	
		Word() = default;
		Word(std::string&& text, std::string&& difficulty)
			: id{ 0 }, text{ std::move(text) }, difficulty{ std::move(difficulty) } 
		{ /* empty */ }
		Word(Word&&) = default;
	};
	
	struct Player
	{
		int id;
		std::string username;
		std::string password;
		bool isOnline;

		Player() = default;
		Player(const std::string& username, const std::string& password)
			: id{ 0 }, username{ username }, password{ password }, isOnline{ false }
		{ /* empty */ }
		Player(std::string&& username, std::string&& password)
			: id{ 0 }, username{ std::move(username) }, password{ std::move(password) }, isOnline{ false }
		{ /* empty */  }
	};
	
	struct GameHistory
	{
		int id;
		int playerID;
		int score;
		std::string difficulty;
		std::string date;

		GameHistory() = default;
		GameHistory(int playerID, int score, const std::string& difficulty, const std::string& date)
			: id{ 0 }, playerID{ playerID }, score{ score }, difficulty{ difficulty }, date{ date }
		{ /* empty */ }
		GameHistory(int playerID, int score, std::string&& difficulty, std::string&& date)
			: id{ 0 }, playerID{ playerID }, score{ score }, difficulty{ difficulty }, date{ date }
		{ /* empty */ }
	};
	
	inline auto CreateStorage(const std::string& filename)
	{
		return sql::make_storage(filename,
			sql::make_table("word",
				sql::make_column("id", &Word::id, sql::primary_key().autoincrement()),
				sql::make_column("word", &Word::text),
				sql::make_column("difficulty", &Word::difficulty)),
			sql::make_table("player",
				sql::make_column("id", &Player::id, sql::primary_key().autoincrement()),
				sql::make_column("username", &Player::username),
				sql::make_column("password", &Player::password),
				sql::make_column("isOnline", &Player::isOnline)),
			sql::make_table("gameHistory",
				sql::make_column("id", &GameHistory::id, sql::primary_key().autoincrement()),
				sql::make_column("playerID", &GameHistory::playerID),
				sql::make_column("score", &GameHistory::score),
				sql::make_column("difficulty", &GameHistory::difficulty),
				sql::make_column("date", &GameHistory::date))
		);
	}
}