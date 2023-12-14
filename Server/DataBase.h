#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <crow.h>
#include <vector>
#include <sqlite_orm/sqlite_orm.h>

#include "DatabasePrerequisites.h"

namespace sql = sqlite_orm;

class Database
{
public:
	Database(const std::string& filename);
	Database(const Database&) = delete;
	Database& operator=(const Database&) = delete;
	~Database() = default;

	void PopulateStorage();
	//void SignInOrUp(const std::string& playerName, const std::string& password);
	bool IfPlayerExist(const std::string& playerName);
	bool SignUp(const std::string& playerName, const std::string& password);
	bool SignIn(const std::string& playerName, const std::string& password);
	bool SignOut(const std::string& playerName);
	void AddGame(const std::string& playerName, int score, const std::string& difficulty, const std::string& date);
	void GetGameHistory(const std::string& playerName);
	std::vector<std::string> GetRandomWords(int number ,const std::string& difficulty);
	
private:
	decltype(db::CreateStorage("")) m_storage;
};
