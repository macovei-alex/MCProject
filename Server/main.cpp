#include "Server.h"
#include "Game.h"

import utilities;

// https://docs.google.com/document/d/17Up9pbhwRiUGXRCoE4aJbuTiBvuXILxOdLOBuDKdr3E/edit

int main()
{
	Server::getInstance().port(18080).allHandlers().run();
	return 0;
}