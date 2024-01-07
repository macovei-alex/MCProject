#pragma once

#include <QThread>
#include <QPair>

#include "common.h"

class GameStateThread : public QThread
{
	Q_OBJECT

public:
	GameStateThread(uint64_t roomID, bool& keepGoing, QWidget* parent = nullptr);
	~GameStateThread() = default;

signals:
	void GameStateSignal(const QPair<common::game::GameState, uint64_t>& gameStatePair);

public:
	uint64_t roomID;
	bool& keepGoing;

public:
	void run() override;
};