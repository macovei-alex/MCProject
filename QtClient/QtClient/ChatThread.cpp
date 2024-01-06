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
	using std::chrono_literals::operator""s;
	try
	{
		while (keepGoing)
		{

			emit ChatSignal(QList<Message>());

			std::this_thread::sleep_for(0.25s);
		}
	}
	catch (const std::exception& e)
	{
		qDebug() << e.what() << '\n';
	}
}
#endif