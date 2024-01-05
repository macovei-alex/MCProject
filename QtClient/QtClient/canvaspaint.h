#ifndef CANVASPAINT_H
#define CANVASPAINT_H

#if defined(_MSVC_LANG) && (_MSVC_LANG == 202002L) && 0
#define ONLINE
#endif

#include <QDialog>
#include <QPainter>
#include <QMouseEvent>
#include <qpixmap.h>
#include <cstdint>

#ifdef ONLINE
#include "services.h"
#include <vector>
#include <QThread>
#endif

class MainWindow;

enum class DrawingState : uint8_t
{
	DRAWING,
	ERASING
};

struct DrawnLine
{
	QList<QPoint> points;
	DrawingState drawState;

	DrawnLine() = default;

#ifdef ONLINE
	DrawnLine(std::vector<common::img::Point>&& points, uint32_t color);
	std::vector<common::img::Point> ToCommonPoints() const;
#endif
};

#ifdef ONLINE
class ImageReceiver : public QObject
{
	Q_OBJECT

public slots:
	void ImageReceiverSlot(uint64_t roomID, QList<DrawnLine>& lines, bool& keepGoing);
};
#endif

namespace Ui {
	class CanvasPaint;
}

class CanvasPaint : public QDialog
{
	Q_OBJECT

public:
	CanvasPaint(QWidget* parent = nullptr);

#ifdef ONLINE
	CanvasPaint(uint64_t roomID, QWidget* parent = nullptr);
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

private:
	MainWindow* signInWindow;

	QPixmap canvasPixmap;
	QList<DrawnLine> drawnLines;
	DrawnLine currentLine;
	QPoint lastPoint;

	DrawingState drawState;
	Ui::CanvasPaint* ui;

#ifdef ONLINE
	uint64_t roomID;
	QThread imageReceiverThread;
	ImageReceiver imageReceiver;
	bool keepGoing;
#endif

private:
	const QPen DRAWING_PEN = QPen(Qt::black, 2, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);
	const QPen ERASING_PEN = QPen(Qt::white, 20, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
};

#endif // CANVASPAINT_H
