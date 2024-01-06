#pragma once

#include "DatabasePrerequisites.h"

namespace db
{
	class MockDatabase
	{
	public:
		MockDatabase(const std::string& filename);
		MockDatabase(const MockDatabase&) = delete;
		MockDatabase& operator=(const MockDatabase&) = delete;
		~MockDatabase() = default;

		void PopulateStorage();
		//void SignInOrUp(const std::string& playerName, const std::string& password);
		bool IfPlayerExist(const std::string& playerName);
		db::ReturnValue SignUp(const std::string& playerName, const std::string& password);
		db::ReturnValue SignIn(const std::string& playerName, const std::string& password);
		db::ReturnValue SignOut(const std::string& playerName);
		db::ReturnValue AddGame(const std::string& playerName, int score, const std::string& difficulty, const std::string& date);
		db::GameHistory GetGameHistory(const std::string& playerName);
		std::vector<std::string> GetRandomWords(int number, const std::string& difficulty);
	};
}
