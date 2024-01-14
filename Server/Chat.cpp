#include "Chat.h"

#include "constantLiterals.h"

Chat::Chat() noexcept:
	m_messages{},
	m_sharedMutex{ std::make_shared<std::mutex>() }
{
	/* empty */
}

void Chat::Add(const common::Message& message)
{
	m_messages.push_back(message);
}

void Chat::Emplace(common::Message&& message)
{
	m_messages.emplace_back(std::move(message));
}

void Chat::Clear()
{
	m_messages.clear();
}

std::vector<common::Message> Chat::GetMessagesOrdered(uint64_t start, const std::string& skipAuthor) const
{
	std::vector<common::Message> messages;
	for (auto it = m_messages.rbegin(); it != m_messages.rend() && it->timestamp > start; it++)
		if (start == 0 || it->author != skipAuthor)
			messages.emplace_back(*it);
	return messages;
}

std::vector<crow::json::wvalue> Chat::GetMessagesOrderedJsonList(uint64_t start, const std::string& skipAuthor) const
{
	std::vector<crow::json::wvalue> messagesJson;
	for (auto it = m_messages.rbegin(); it != m_messages.rend() && it->timestamp > start; it++)
		if (start == 0 || it->author != skipAuthor)
			messagesJson.emplace_back(crow::json::wvalue{
				{ literals::jsonKeys::message::author, it->author },
				{ literals::jsonKeys::message::text, it->text },
				{ literals::jsonKeys::message::timestamp, it->timestamp } });
	return messagesJson;
}

size_t Chat::Size() const noexcept
{
	return m_messages.size();
}

bool Chat::Empty() const noexcept
{
	return m_messages.empty();
}

common::Message& Chat::operator[](size_t index)
{
	return m_messages[index];
}

std::mutex& Chat::GetMutex()
{
	return *m_sharedMutex;
}
