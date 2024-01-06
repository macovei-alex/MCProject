#pragma once
#include "macro.h"

#include <QString>

struct Message
{
	QString text;
	QString author;
	uint64_t timestamp;

	Message();
	Message(const QString& text, const QString& author, uint64_t timestamp = 0);
	Message(QString&& text, QString&& author, uint64_t timestamp = 0);
	Message(const Message& other);
    Message& operator=(const Message& other);

#ifdef ONLINE
    Message(Message&& other);
	Message& operator=(Message&& other);
#endif
};
