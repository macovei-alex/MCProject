#pragma once

#include <QThread>
#include <QPair>

#include "GameState.h"

class GameStateThread : public QThread
{
	Q_OBJECT

public:
	GameStateThread(uint64_t roomID, bool& keepGoing, QWidget* parent = nullptr);
	~GameStateThread() = default;

signals:
	void GameStateSignal(const QPair<GameState, uint64_t>& gameStatePair);

public:
	uint64_t roomID;
	bool& keepGoing;

public:
	void run() override;
};