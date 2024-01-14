#include "ChatThread.h"
#include "macro.h"

#ifdef ONLINE
#include <QDebug>
#include <QWidget>

#include "services.h"

ChatThread::ChatThread(uint64_t roomID, const QString& username, bool& keepGoing, QWidget* parent) :
	QThread{ parent },
	keepGoing{ keepGoing },
	roomID{ roomID },
	username{ username }
{
	/* empty */
}

void ChatThread::run()
{

	while (keepGoing)
	{
		if (IsPaused())
		{
			QThread::msleep(1000);
			continue;
		}

		try
		{
			while (!IsPaused() && keepGoing)
			{
				QList<common::Message> messagesQList;

				auto messagesVector{ services::ReceiveNewMessages(username.toStdString(), roomID) };
				for(const auto& message : messagesVector)
					messagesQList.push_back(common::Message{ message.text, message.author });

				emit ChatSignal(messagesQList);

				QThread::msleep(1000);
			}
		}
		catch (const std::exception& e)
		{
			qDebug() << e.what() << '\n';
		}
	}
}
#endif