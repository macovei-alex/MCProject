#pragma once

#include <QThread>
#include <QPair>

#include "GameState.h"

class GameStateReceiverThread : public QThread
{
	Q_OBJECT

public:
	GameStateReceiverThread(uint64_t roomID, bool& keepGoing, QWidget* parent = nullptr);
	~GameStateReceiverThread() = default;

signals:
	void GameStateReceivedSignal(const QPair<GameState, uint64_t>& gameStatePair);

public:
	uint64_t roomID;
	bool& keepGoing;

public:
	void run() override;
};