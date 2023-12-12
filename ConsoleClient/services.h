#pragma once

#include <cstdint>
#include <cpr/cpr.h>
#include "clientUtils.h"

#ifndef _TEMP
#define _TEMP

struct Color
{
	uint8_t r;
	uint8_t g;
	uint8_t b;

	Color() = default;
	Color(int32_t rgbHex);
	int32_t ToInt32() const;
	bool operator==(const Color& other) const = default;
};

struct Point
{
	int16_t x;
	int16_t y;
	Color color;
	bool operator==(const Point& other) const = default;
};

#endif // _TEMP

namespace services
{
	uint64_t CreateRoom();
	bool ConnectToRoom(uint64_t roomID);
	bool SignIn(const std::string& username, const std::string& password);
	bool SignUp(const std::string& username, const std::string& password);
	bool SignOut(const std::string& username);
	void SendNewMessage(std::ostream& errStream, const std::string& username, const std::string& content, uint64_t gameID);
	void ReceiveNewMessages(std::ostream& outputStream, const std::string& username, uint64_t gameID);
	void SendImageUpdates(std::ostream& errStream, uint64_t gameID, const std::vector<Point>& points);
	void ReceiveImageUpdates(std::ostream& outputStream, uint64_t gameID);

	void MessageSender(uint64_t gameID, const std::string& username, bool* keepGoing);
	void MessagesReceiver(uint64_t gameID, const std::string& username, bool* keepGoing);
	void ImageUpdatesReceiver(uint64_t gameID, bool* keepGoing);
}