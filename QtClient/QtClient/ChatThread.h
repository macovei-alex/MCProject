#pragma once
#include "macro.h"

#include <QThread>

#include "ThreadExtension.h"
#include "common.h"

class ChatThread : public QThread, public ThreadExtension
{
	Q_OBJECT

public:
	ChatThread(uint64_t roomID, const QString& username, bool& keepGoing, QWidget* parent = nullptr);
	~ChatThread() = default;

signals:
	void ChatSignal(const QList<common::Message>&);

public:
	uint64_t roomID;
	bool& keepGoing;
	QString username;

public:
	void run() override;
};