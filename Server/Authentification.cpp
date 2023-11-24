#include "Authentification.h"

bool Autentificare::Register(const std::string& username, const std::string& password)
{
	auto players = m_dataBase.get_all<PlayerDB>(sql::where(sql::is_equal(&PlayerDB::playerName, username)));
	if (players.empty())
	{
		/* Insert user into the db */
		return true;
	}
	return false;
}

bool Autentificare::Login(const std::string& username, const std::string& password)
{
	auto player = std::move(m_dataBase.get_all<PlayerDB>(sql::where(sql::is_equal(&PlayerDB::playerName, username))));
	auto playerPassword = std::move(m_dataBase.get_all<PlayerDB>(sql::where(sql::is_equal(&PlayerDB::password, password))));

	if (player.size() == 0 || playerPassword.size() == 0)
	{
		return false;
	}
	else
	{
		return false;
	}
}

Autentificare::Autentificare(Storage& dataBase) : m_dataBase(dataBase)
{
	/*EMPTY*/
}

Autentificare::~Autentificare()
{
	/*EMPTY*/
}
