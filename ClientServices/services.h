#pragma once

#include "servicesUtils.h"

#include <cstdint>
#include <cpr/cpr.h>
#include <iostream>

#include "common.h"
#include "GameSettings.h"
#include "..\TemporaryLogger\logger.h"

namespace services
{
	uint64_t CreateRoom(std::ostream& outStream = std::cout, std::ostream& errStream = std::cerr);
	bool ConnectToRoom(uint64_t roomID, std::ostream& outStream = std::cout, std::ostream& errStream = std::cerr);

	bool SignIn(const std::string& username, const std::string& password, std::ostream& outStream = std::cout, std::ostream& errStream = std::cerr);
	bool SignUp(const std::string& username, const std::string& password, std::ostream& outStream = std::cout, std::ostream& errStream = std::cerr);
	bool SignOut(const std::string& username, std::ostream& outStream = std::cout, std::ostream& errStream = std::cerr);

	void SendNewMessage(const std::string& username, const std::string& content, uint64_t gameID, std::ostream& outStream = std::cout, std::ostream& errStream = std::cerr);
	void ReceiveNewMessages(const std::string& username, uint64_t gameID, std::ostream& outStream = std::cout, std::ostream& errStream = std::cerr);

	void SendImageUpdates(uint64_t gameID, const std::vector<common::img::Point>& points, std::ostream& outStream = std::cout, std::ostream& errStream = std::cerr);
	void ReceiveImageUpdates(uint64_t gameID, std::ostream& outStream = std::cout, std::ostream& errStream = std::cerr);

	void SendGameSettings(uint64_t gameID, const GameSettings& gameSettings, std::ostream& outStream = std::cout, std::ostream& errStream = std::cerr);
	GameSettings ReceiveGameSettings(uint64_t gameID, std::ostream& outStream = std::cout, std::ostream& errStream = std::cerr);

	void MessageSender(uint64_t gameID, const std::string& username, bool* keepGoing);
	void MessagesReceiver(uint64_t gameID, const std::string& username, bool* keepGoing);
	void ImageUpdatesReceiver(uint64_t gameID, bool* keepGoing);

	static Logger* logger = nullptr;
	void SetLogFile(const std::string& filename);
	void Log(const std::string_view& message, Logger::Level level = Logger::Level::Info);
}