#pragma once
#include "macro.h"

#include <QThread>

#include "Line.h"
#include "ThreadExtension.h"

class ImageThread : public QThread, public ThreadExtension
{
	Q_OBJECT

public:
	ImageThread(uint64_t roomID, bool& keepGoing, QWidget* parent = nullptr);
	~ImageThread() = default;

signals:
	void ImageSignal(QList<Line>* lines);

public:
	uint64_t roomID;
	bool& keepGoing;

public:
	void run() override;
};