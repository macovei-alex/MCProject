#ifndef CANVASPAINT_H
#define CANVASPAINT_H

#include "macro.h"

#include <QDialog>
#include <QPainter>
#include <QMouseEvent>
#include <QThread>
#include <QPixmap>
#include <QLabel>

#include "Line.h"
#include "common.h"

#ifdef ONLINE
#include "ImageThread.h"
#include "GameStateThread.h"
#include "ChatThread.h"
#include "OnlineData.h"
#endif

class MainWindow;

namespace Ui {
	class CanvasPaint;
}

class CanvasPaint : public QDialog
{

	Q_OBJECT

public:
	CanvasPaint(QWidget* parent = nullptr);

#ifdef ONLINE
	CanvasPaint(uint64_t roomID, const QString& username, QWidget* parent = nullptr);
#endif

	~CanvasPaint();

public:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

public:
	void setRoomID(uint64_t roomID);
	void ClearCanvas();

protected:
	void paintEvent(QPaintEvent* event) override;
	void closeEvent(QCloseEvent* event) override;

private slots:
	void on_resetCanvas_clicked();
	void on_drawButton_clicked();
	void on_eraseButton_clicked();
	void on_undoButton_clicked();
	void on_messageButton_clicked();
	void on_startGameButton_clicked();

	void HandleImage(QList<Line>* newLines);
	void HandleGameState(const QPair<common::game::GameState, uint64_t>& gameStatePair);
	void HandleChat(const QList<common::Message>& messages);

	void SetChosenWord(const QString& word);

public:

#ifdef ONLINE
	OnlineData& GetOnlineData();
#endif

signals:
	void Signal();

private:
	void SetAllButtonsEnabled(bool enabled);
	void SetAllThreadsPauseStatus(bool paused);

	MainWindow* signInWindow;

	QPixmap canvasPixmap;
	QList<Line> lines;
	Line currentLine;
	QPoint lastPoint;
	QLabel* roomLabel;

	DrawingState m_drawState;
	Ui::CanvasPaint* ui;
	uint64_t roomID;

#ifdef ONLINE
	OnlineData m_onlineData;
	ImageThread* m_imageThread;
	GameStateThread* m_gameStateThread;
	ChatThread* m_chatThread;
	bool m_keepGoing;
#endif

private:
	const QPen kDRAWING_PEN = QPen{ QColor{ static_cast<QRgb>(Line::kDRAWING_COLOR_INT) }, 2, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin };
	const QPen kERASING_PEN = QPen{ QColor{ static_cast<QRgb>(Line::kERASING_COLOR_INT) }, 20, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin };
};

#endif // CANVASPAINT_H
