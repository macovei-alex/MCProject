#pragma once
#include <string>
#include "Database.h"

class Autentificare {
public:
	Autentificare(Storage& dataBase);
	~Autentificare();
	Autentificare(const Autentificare& autentificare) = delete;
	Autentificare& operator=(const Autentificare&) = delete;
	bool Register(const std::string& username, const std::string& password);
	bool Login(const std::string& username, const std::string& password);

private:
	Storage& m_dataBase;
};