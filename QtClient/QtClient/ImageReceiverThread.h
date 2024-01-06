#pragma once

#include <QThread>

#include "MyLine.h"

class ImageReceiverThread : public QThread
{
	Q_OBJECT

public:
	ImageReceiverThread(uint64_t roomID, bool& keepGoing, QWidget* parent = nullptr);
	~ImageReceiverThread() = default;

signals:
	void LinesReceivedSignal(QList<MyLine>* lines);

public:
	uint64_t roomID;
	bool& keepGoing;

public:
	void run() override;
};