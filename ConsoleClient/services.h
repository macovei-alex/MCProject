#pragma once

#include <cstdint>
#include <cpr/cpr.h>
#include "clientUtils.h"

namespace services
{
	uint64_t CreateRoom();
	bool ConnectToRoom(uint64_t roomID);
	bool SignIn(const std::string& username, const std::string& password);
	bool SignUp(const std::string& username, const std::string& password);
	bool SignOut(const std::string& username);
	void SendNewMessage(std::ostream& outputStream, const std::string& username, const std::string& content, uint64_t gameID);
	void ReceiveNewMessages(std::ostream& outputStream, const std::string& username, uint64_t gameID);
	void ReceiveImageUpdates(std::ostream& outputStream, uint64_t gameID);

	void MessageSender(uint64_t gameID, const std::string& username, bool* keepGoing);
	void MessagesReceiver(uint64_t gameID, const std::string& username, bool* keepGoing);
	void ImageUpdatesReceiver(uint64_t gameID, bool* keepGoing);
}