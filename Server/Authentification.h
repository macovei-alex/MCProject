#pragma once
#include <string>
#include "database.h"
class Autentificare {
public:
	crow::response Register(std::string username, std::string password);
	crow::response Login(std::string username, std::string password);
private:
	Storage& m_dataBase;
};