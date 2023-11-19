#pragma once
#include <string>
#include "database.h"
class Autentificare {
public:
	Autentificare(Storage& dataBase);
	~Autentificare();
	Autentificare( const Autentificare& autentificare ) = delete;
	Autentificare& operator=( const Autentificare& ) = delete;
	crow::response Register(std::string username, std::string password);
	crow::response Login(std::string username, std::string password);
	
private:
	Storage& m_dataBase;
};