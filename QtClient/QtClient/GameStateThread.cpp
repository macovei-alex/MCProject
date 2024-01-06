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
	using std::chrono_literals::operator""s;
	try
	{
		while (keepGoing)
		{
			auto gameStatePair{ services::ReceiveGameStateAndTimer(roomID) };
			auto gameStateQPair{ QPair{ static_cast<GameState>(gameStatePair.first), gameStatePair.second } };

			qDebug() << "Received game state and timer: "
				<< static_cast<uint16_t>(gameStateQPair.first) << ' ' << gameStateQPair.second;

			emit GameStateSignal(gameStateQPair);

			std::this_thread::sleep_for(0.25s);
		}
	}
	catch (const std::exception& e)
	{
		qDebug() << e.what() << '\n';
	}
}
#endif