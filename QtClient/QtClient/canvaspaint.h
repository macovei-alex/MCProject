#ifndef CANVASPAINT_H
#define CANVASPAINT_H

#include <QDialog>
#include <QPainter>
#include <QMouseEvent>
#include <qpixmap.h>

class MainWindow;

namespace Ui {
	class CanvasPaint;
}

class CanvasPaint : public QDialog
{
	Q_OBJECT

public:
	enum class DrawState : uint8_t {
		DRAWING,
		ERASING
	};

public:
	CanvasPaint(QWidget* parent = nullptr);
	~CanvasPaint();
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void resizeEvent(QResizeEvent* event) override;
	void clearCanvas();
	void setDrawState(DrawState state);

	QPixmap canvasPixmap;

protected:
	void paintEvent(QPaintEvent* event) override; // Asigură-te că override este specificat aici
	void closeEvent(QCloseEvent* event) override;

private slots:
	void on_button_clicked();
	void on_leaveServerButton_clicked();
	void on_resetCanvas_clicked();
	void on_drawButton_clicked();
	void on_eraseButton_clicked();
	void on_undoButton_clicked();
	void on_messageButton_clicked();

private:
	struct DrawnLine {
		bool isDrawing; // true pentru desenare, false pentru ștergere
		QList<QPoint> points;
		DrawState drawState;
	};

	DrawState drawState;

	bool drawingOrErasing;
	Ui::CanvasPaint* ui;
	QPoint lastPoint;
	MainWindow* obiect;
	QList<DrawnLine> drawnLines;
	DrawnLine currentLine;

	const QPen drawingPen = QPen(Qt::black);
	const QPen erasingPen = QPen(Qt::white, 20, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
};

#endif // CANVASPAINT_H
