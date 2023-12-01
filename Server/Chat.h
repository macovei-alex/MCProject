#pragma once

#include <vector>
#include <crow.h>
#include <stack>

#include "utilitiesModTemp.h"
// import utilitiesMod;

class Chat
{
public:
	Chat() noexcept = default;
	Chat(const Chat& other) noexcept = default;
	Chat& operator=(const Chat& other) noexcept = default;
	Chat(Chat&& other) noexcept = default;
	Chat& operator= (Chat&& other) noexcept = default;
	~Chat() noexcept = default;

public:
	void Add(const utils::Message& message);
	void Emplace(utils::Message&& message);
	std::vector<utils::Message> GetMessagesOrdered(uint64_t start) const;
	std::vector<crow::json::wvalue> GetMessagesOrderedJson(uint64_t start) const;
	size_t Size() const noexcept;
	bool Empty() const noexcept;
	utils::Message& operator[](size_t index);

private:
	std::vector<utils::Message> m_messages;
};
