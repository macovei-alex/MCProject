#include "Server.h"
#include "Game.h"

import utilities;

// https://docs.google.com/document/d/17Up9pbhwRiUGXRCoE4aJbuTiBvuXILxOdLOBuDKdr3E/edit

int main()
{
	Server server;
	server.allHandlers().port(18080).run();

	return 0;
}