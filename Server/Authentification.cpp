#include "Authentification.h"

crow::response Autentificare::Register(std::string username, std::string password)
{
    return crow::response();
}

crow::response Autentificare::Login(std::string username, std::string password)
{    auto player = m_dataBase.get_all<TablePlayer>(sql::where(sql::is_equal(&TablePlayer::playerName, username)));
     auto playerPassword = m_dataBase.get_all<TablePlayer>
         (sql::where(sql::is_equal(&TablePlayer::password, password)));
     if (player.size() == 1 && playerPassword.size() > 0)
		 return crow::response(200);

     return crow::response(404); 
}
