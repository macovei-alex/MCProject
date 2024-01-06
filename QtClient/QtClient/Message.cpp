#include "Message.h"

Message::Message() :
	text{ "" },
	author{ "" },
	timestamp{ 0 }
{
	/* empty */
}

Message::Message(const QString& text, const QString& author, uint64_t timestamp) :
	text{ text },
	author{ author },
	timestamp{ timestamp }
{
	/* empty */
}

Message::Message(QString&& text, QString&& author, uint64_t timestamp) :
	text{ std::move(text) },
	author{ std::move(author) },
	timestamp{ timestamp }
{
	/* empty */
}

Message::Message(const Message& other) :
	text{ other.text },
	author{ other.author },
	timestamp{ other.timestamp }
{
	/* empty */
}

Message::Message(Message&& other) noexcept :
	text{ std::move(other.text) },
	author{ std::move(other.author) },
	timestamp{ other.timestamp }
{
	/* empty */
}

Message& Message::operator=(const Message& other)
{
	if (this != &other)
	{
		text = other.text;
		author = other.author;
		timestamp = other.timestamp;
	}

	return *this;
}

Message& Message::operator=(Message&& other) noexcept
{
	if (this != &other)
	{
		text = std::move(other.text);
		author = std::move(other.author);
		timestamp = other.timestamp;
	}

	return *this;
}