#pragma once

#include "DatabasePrerequisites.h"

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
	bool SignUp(const std::string& playerName, const std::string& password);
	bool SignIn(const std::string& playerName, const std::string& password);
	bool SignOut(const std::string& playerName);
	void AddGame(const std::string& playerName, int score, const std::string& difficulty, const std::string& date);
	void GetGameHistory(const std::string& playerName);
	std::vector<std::string> GetRandomWords(int number, const std::string& difficulty);
};

