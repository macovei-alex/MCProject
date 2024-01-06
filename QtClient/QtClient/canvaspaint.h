#ifndef CANVASPAINT_H
#define CANVASPAINT_H

#include "macro.h"

#include <QDialog>
#include <QPainter>
#include <QMouseEvent>
#include <QThread>
#include <qpixmap.h>
#include <cstdint>

#include "MyLine.h"
#include "ImageReceiverThread.h"
#include "GameStateReceiverThread.h"

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

	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

	void ClearCanvas();

protected:
	void paintEvent(QPaintEvent* event) override;
	void closeEvent(QCloseEvent* event) override;

private slots:
	void on_leaveServerButton_clicked();
	void on_resetCanvas_clicked();
	void on_drawButton_clicked();
	void on_eraseButton_clicked();
	void on_undoButton_clicked();
	void on_messageButton_clicked();

	void HandleAddLines(QList<MyLine>* newLines);
	void HandleReceiveState(const std::pair<common::game::GameState, uint64_t>& gameStatePair);

signals:
	void Signal();

private:
	MainWindow* signInWindow;

	QPixmap canvasPixmap;
	QList<MyLine> myLines;
	MyLine currentLine;
	QPoint lastPoint;

	DrawingState drawState;
	Ui::CanvasPaint* ui;

#ifdef ONLINE
	uint64_t roomID;
	ImageReceiverThread* imageReceiverThread;
	GameStateReceiverThread* gameStateReceiverThread;
	bool keepGoing;
	QString username;
#endif

private:
	const QPen DRAWING_PEN = QPen{ QColor{ static_cast<QRgb>(MyLine::DRAWING_COLOR_INT) }, 2, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin };
	const QPen ERASING_PEN = QPen{ QColor{ static_cast<QRgb>(MyLine::ERASING_COLOR_INT) }, 20, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin };
};

#endif // CANVASPAINT_H
