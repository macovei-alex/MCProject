#pragma once

#include <cstdint>
#include <cpr/cpr.h>
#include "clientUtils.h"

namespace handlers
{
	uint64_t CreateRoom();
	bool ConnectToRoom(uint64_t roomID);
	bool SignIn(const std::string& username, const std::string& password);
	bool SignUp(const std::string& username, const std::string& password);
	bool SignOut(const std::string& username);
	void SingleChatLoadHandler(std::ostream& outputStream, cpr::Response& response, uint64_t& lastTimeMillis);

	void Sender(uint64_t gameID, const std::string& username, bool* keepGoing);
	void Receiver(uint64_t gameID, const std::string& username, bool* keepGoing);
}