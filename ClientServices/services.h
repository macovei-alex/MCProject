#pragma once

#include <cstdint>
#include <cpr/cpr.h>
#include <iostream>

#include "common.h"

namespace services
{
	uint64_t CreateRoom(const std::string& username, std::ostream& outStream = std::cout, std::ostream& errStream = std::cerr);
	bool ConnectToRoom(uint64_t roomID, const std::string& username, std::ostream& outStream = std::cout, std::ostream& errStream = std::cerr);

	bool SignIn(const std::string& username, const std::string& password, std::ostream& outStream = std::cout, std::ostream& errStream = std::cerr);
	bool SignUp(const std::string& username, const std::string& password, std::ostream& outStream = std::cout, std::ostream& errStream = std::cerr);
	bool SignOut(const std::string& username, std::ostream& outStream = std::cout, std::ostream& errStream = std::cerr);

	void SendNewMessage(const std::string& username, const std::string& content, uint64_t gameID, std::ostream& outStream = std::cout, std::ostream& errStream = std::cerr);
	std::vector<common::Message> ReceiveNewMessages(const std::string& username, uint64_t gameID, std::ostream& outStream = std::cout, std::ostream& errStream = std::cerr);

	void SendImageUpdates(uint64_t gameID, const std::vector<common::img::Point>& points, std::ostream& outStream = std::cout, std::ostream& errStream = std::cerr);
	std::vector<common::img::Point> ReceiveImageUpdates(uint64_t gameID, std::ostream& errStream = std::cerr);

	std::pair<common::game::GameState, uint64_t> ReceiveGameStateAndTime(uint64_t gameID, std::ostream& errStream = std::cerr);
	common::game::PlayerRole ReceivePlayerRole(uint64_t roomID, const std::string& username, std::ostream& errStream = std::cerr);
	std::vector<std::pair<std::string, int32_t>> ReceivePlayerScores(uint64_t gameID, std::ostream& errStream = std::cerr);

	void SendGameSettings(uint64_t gameID, const common::game::GameSettings& gameSettings, std::ostream& outStream = std::cout, std::ostream& errStream = std::cerr);
	common::game::GameSettings ReceiveGameSettings(uint64_t gameID, std::ostream& errStream = std::cerr);
}