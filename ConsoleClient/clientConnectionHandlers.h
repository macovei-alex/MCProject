#pragma once

#include <cstdint>
#include "clientUtils.h"

namespace handlers
{
	uint64_t CreateRoom();
	bool ConnectToRoom(uint64_t roomID);
	bool SignIn(const std::string& username, const std::string& password);
	bool SignUp(const std::string& username, const std::string& password);
	bool SignOut(const std::string& username);

	void Sender(uint64_t gameID, const std::string& username, bool* keepGoing);
	void Receiver(uint64_t gameID, const std::string& username, bool* keepGoing);
}