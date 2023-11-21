#include "Server.h"
#include "Game.h"

import utilities;

// https://docs.google.com/document/d/17Up9pbhwRiUGXRCoE4aJbuTiBvuXILxOdLOBuDKdr3E/edit

int main()
{
	Server::getInstance().IPAddress("127.0.0.1").port(18080).allHandlers().run();
	return 0;
}