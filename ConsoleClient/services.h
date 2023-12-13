#pragma once

#include "clientUtils.h"
#include "../Common/commonUtils.h"
#include "../Common/GameSettings.h"

#include <cstdint>
#include <cpr/cpr.h>

namespace services
{
	uint64_t CreateRoom();
	bool ConnectToRoom(uint64_t roomID);

	bool SignIn(const std::string& username, const std::string& password);
	bool SignUp(const std::string& username, const std::string& password);
	bool SignOut(const std::string& username);

	void SendNewMessage(std::ostream& errStream, const std::string& username, const std::string& content, uint64_t gameID);
	void ReceiveNewMessages(std::ostream& outputStream, const std::string& username, uint64_t gameID);

	void SendImageUpdates(std::ostream& errStream, uint64_t gameID, const std::vector<utils::img::Point>& points);
	void ReceiveImageUpdates(std::ostream& outputStream, uint64_t gameID);

	void SendGameSettings(std::ostream& errStream, uint64_t gameID, const GameSettings& gameSettings);
	std::tuple<uint16_t, uint16_t, uint16_t> ReceiveGameSettings(std::ostream& outputStream, uint64_t gameID);

	void MessageSender(uint64_t gameID, const std::string& username, bool* keepGoing);
	void MessagesReceiver(uint64_t gameID, const std::string& username, bool* keepGoing);
	void ImageUpdatesReceiver(uint64_t gameID, bool* keepGoing);
}