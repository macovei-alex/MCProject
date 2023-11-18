#pragma once
#include <string>
#include <vector>
#include <crow.h>
#include <sqlite_orm/sqlite_orm.h>
namespace sql = sqlite_orm;

struct Words
{
	int id;
    std::string word;
	int difficulty;
};

struct Player
{
	std::string playerName;//primary key
	std::string password;
	int gamesPlayed;
};

struct GameHistory
{
	int id;
	std::string playerName;//foreign key
	int score;
	int difficulty;
	std::string date;
};