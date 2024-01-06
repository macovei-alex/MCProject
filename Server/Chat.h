#pragma once

#include <vector>
#include <crow.h>
#include <stack>

#include "common.h"

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
	void Add(const common::Message& message);
	void Emplace(common::Message&& message);
	std::vector<common::Message> GetMessagesOrdered(uint64_t start, const std::string& skipAuthor) const;
	std::vector<crow::json::wvalue> GetMessagesOrderedJsonList(uint64_t start, const std::string& skipAuthor) const;
	size_t Size() const noexcept;
	bool Empty() const noexcept;
	common::Message& operator[](size_t index);

private:
	std::vector<common::Message> m_messages;
};
