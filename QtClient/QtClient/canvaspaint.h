#ifndef CANVASPAINT_H
#define CANVASPAINT_H

#if defined(_MSVC_LANG) && (_MSVC_LANG == 202002L) && 0
#define ONLINE
#endif

#include <QDialog>
#include <QPainter>
#include <QMouseEvent>
#include <qpixmap.h>

#ifdef ONLINE
#include "services.h"
#include <vector>
#endif

class MainWindow;

namespace Ui {
	class CanvasPaint;
}

class CanvasPaint : public QDialog
{
	Q_OBJECT

public:
	enum class DrawingState : uint8_t {
		DRAWING,
		ERASING
	};

	struct DrawnLine
	{
		QList<QPoint> points;
		DrawingState drawState;
	};

public:
	CanvasPaint(QWidget* parent = nullptr);
	~CanvasPaint();

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

	void clearCanvas();
	void setDrawState(DrawingState state);

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

private:
	const QPen DRAWING_PEN = QPen(Qt::black, 2, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);
	const QPen ERASING_PEN = QPen(Qt::white, 20, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

private:
#ifdef ONLINE
	std::vector<common::img::Point> convertToCommonPoints(const DrawnLine& points);
#endif
};

#endif // CANVASPAINT_H
