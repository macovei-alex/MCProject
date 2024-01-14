#include "GameStateThread.h"
#include "macro.h"

#ifdef ONLINE
#include <QDebug>
#include <QWidget>

#include "services.h"

GameStateThread::GameStateThread(uint64_t roomID, bool& keepGoing, QWidget* parent) :
	QThread{ parent },
	keepGoing{ keepGoing },
	roomID{ roomID }
{
	/* empty */
}

void GameStateThread::run()
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
				auto gameStatePair{ services::ReceiveGameStateAndTime(roomID) };
				auto gameStateQPair{ QPair{ static_cast<common::game::GameState>(gameStatePair.first), gameStatePair.second } };

				emit GameStateSignal(gameStateQPair);

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