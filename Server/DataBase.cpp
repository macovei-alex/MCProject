#include "DataBase.h"
#include <fstream>

void populateStorage(Storage& storage)
{
	std::ifstream f("words.txt");
	std::vector<Words> words;
	int index = 0;
	while (!f.eof())
	{
		index++;
		std::string word;
		std::string difficulty;
		f >> word >> difficulty;
		words.push_back(Words(index, word, difficulty));
	}
	storage.insert_range(words.begin(), words.end());
};

MakePlayerAccount::MakePlayerAccount(Storage& storage)
	: m_db(storage)
{
	/*EMPTY*/
}
