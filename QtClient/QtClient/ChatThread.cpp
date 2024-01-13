#include "ChatThread.h"
#include "macro.h"

#ifdef ONLINE
#include <QDebug>
#include <QWidget>

#include "services.h"

ChatThread::ChatThread(uint64_t roomID, bool& keepGoing, QWidget* parent) :
	QThread{ parent },
	keepGoing{ keepGoing },
	roomID{ roomID }
{
	/* empty */
}

void ChatThread::run()
{

	while (keepGoing)
	{
		if (IsPaused())
		{
			QThread::msleep(500);
			continue;
		}

		try
		{
			while (!IsPaused() && keepGoing)
			{

				emit ChatSignal(QList<common::Message>());

				QThread::msleep(500);
			}
		}
		catch (const std::exception& e)
		{
			qDebug() << e.what() << '\n';
		}
	}
}
#endif