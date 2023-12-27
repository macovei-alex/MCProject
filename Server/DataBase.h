#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <crow.h>
#include <vector>
#include <sqlite_orm/sqlite_orm.h>

#include "DatabasePrerequisites.h"

namespace sql = sqlite_orm;

namespace db
{
	using Storage = decltype(db::CreateStorage(""));

	class Database
	{
	public:
		Database(const std::string& filename);
		Database(const Database&) = delete;
		Database& operator=(const Database&) = delete;
		~Database() = default;

		void PopulateStorage();
		//void SignInOrUp(const std::string& playerName, const std::string& password);
		db::ReturnValue SignUp(const std::string& playerName, const std::string& password);
		db::ReturnValue SignIn(const std::string& playerName, const std::string& password);
		db::ReturnValue SignOut(const std::string& playerName);
		void AddGame(const std::string& playerName, int score, const std::string& difficulty, const std::string& date);
		void GetGameHistory(const std::string& playerName);
		std::vector<std::string> GetRandomWords(int number, const std::string& difficulty);

	private:
		Storage m_storage;
		bool IfPlayerExist(const std::string& playerName);
	};
}
