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

	struct DrawnLine
	{
		QList<QPoint> points;
		DrawState drawState;
	};

public:
	CanvasPaint(QWidget* parent = nullptr);
	~CanvasPaint();

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

	void clearCanvas();
	void setDrawState(DrawState state);

protected:
	void paintEvent(QPaintEvent* event) override; // Asigură-te că override este specificat aici
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

	DrawState drawState;
	Ui::CanvasPaint* ui;

private:
	const QPen DRAWING_PEN = QPen(Qt::black, 2, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin);
	const QPen ERASING_PEN = QPen(Qt::white, 20, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
};

#endif // CANVASPAINT_H
