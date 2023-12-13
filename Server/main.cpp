#include "Server.h"

// https://docs.google.com/document/d/17Up9pbhwRiUGXRCoE4aJbuTiBvuXILxOdLOBuDKdr3E/edit

int main()
{
	Server::GetInstance().SetSettingsFromFile("serverSettings.txt").Run();
	return 0;
}