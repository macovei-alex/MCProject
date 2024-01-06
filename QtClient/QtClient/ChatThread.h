#pragma once

#include <QThread>

#include "Message.h"

class ChatThread : public QThread
{
	Q_OBJECT

public:
	ChatThread(uint64_t roomID, bool& keepGoing, QWidget* parent = nullptr);
	~ChatThread() = default;

signals:
	void ChatSignal(QList<Message>);

public:
	uint64_t roomID;
	bool& keepGoing;

public:
	void run() override;
};