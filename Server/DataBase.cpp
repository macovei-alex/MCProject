#include "database.h"
#include <fstream>

void PopulateStorage(Storage& storage)
{
	std::ifstream f("words.txt");
	std::vector<Word> words;
	int index = 0;
	while (!f.eof())
	{
		index++;
		std::string word;
		std::string difficulty;
		f >> word >> difficulty;
		words.push_back(Word(index, word, difficulty));
	}
	storage.insert_range(words.begin(), words.end());
};

MakePlayerAccount::MakePlayerAccount(Storage& storage)
	: m_db(storage)
{
	/*EMPTY*/
}
