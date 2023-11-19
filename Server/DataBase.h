#pragma once
#include <string>
#include <vector>
#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>
namespace sql = sqlite_orm;

struct Word
{
	int id;
    std::string text;
	std::string difficulty;
};

struct TablePlayer
{
	std::string playerName;//primary key
	std::string password;
	int gamesPlayed;
	int totalScore;
};

struct GameHistory
{
	int id;
	std::string playerName;//foreign key
	int score;
	std::string difficulty;
	std::string date;
};

inline auto createStorage(const std::string& filename)
{
	return sql::make_storage(filename,
		sql::make_table("words",
			sql::make_column("id", &Word::id, sql::primary_key().autoincrement()),
			sql::make_column("word", &Word::text),
			sql::make_column("difficulty", &Word::difficulty)),
		sql::make_table("player",
			sql::make_column("playerName", &TablePlayer::playerName, sql::primary_key()),
			sql::make_column("password", &TablePlayer::password),
			sql::make_column("gamesPlayed", &TablePlayer::gamesPlayed),
			sql::make_column("totalScore", &TablePlayer::totalScore)),
		sql::make_table("gameHistory",
			sql::make_column("id", &GameHistory::id, sql::primary_key().autoincrement()),
			sql::make_column("playerName", &GameHistory::playerName),
			sql::foreign_key(&GameHistory::playerName).references(&TablePlayer::playerName),
			sql::make_column("score", &GameHistory::score),
			sql::make_column("difficulty", &GameHistory::difficulty),
			sql::make_column("date", &GameHistory::date))
		);
}

using Storage = decltype(createStorage(""));

void populateStorage(Storage& storage);

class MakePlayerAccount
{
public:
	MakePlayerAccount(Storage& storage);

	crow::response operator()(const crow::request& req) const;

private:
	Storage& m_db;
};