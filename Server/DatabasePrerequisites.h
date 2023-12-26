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
	
	inline auto CreateStorage(const std::string& filename)
	{
		return sql::make_storage(filename,
			sql::make_table("word",
				sql::make_column("id", &Word::id, sql::primary_key().autoincrement()),
				sql::make_column("word", &Word::text),
				sql::make_column("difficulty", &Word::difficulty)),
			sql::make_table("player",
				sql::make_column("id", &Player::id, sql::primary_key().autoincrement()),
				sql::make_column("playerName", &Player::playerName),
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