#include "canvaspaint.h"
#include "mainwindow.h"
#include "ui_canvaspaint.h"

#include <QGuiApplication>
#include <QScreen>

CanvasPaint::CanvasPaint(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::CanvasPaint),
	drawState{ DrawingState::DRAWING }
{
	ui->setupUi(this);

	ui->gameChatLabel->setStyleSheet("border: none;");
	ui->gameChat->setStyleSheet("QWidget { border: 1px solid black; }");

	QScreen* primaryScreen = QGuiApplication::primaryScreen();
	QSize screenSize = primaryScreen->geometry().size();
	setGeometry(0, 0, screenSize.width(), screenSize.height());
	setStyleSheet("QDialog { border: 2px solid black; }");

	canvasPixmap = QPixmap(screenSize.width() * 3 / 4, screenSize.height());
	canvasPixmap.fill(Qt::white);

	setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
}

CanvasPaint::~CanvasPaint()
{
	delete ui;
}

void CanvasPaint::setDrawState(DrawingState state)
{
	drawState = state;
}

void CanvasPaint::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	QPainter painter(this);
	painter.setPen(DRAWING_PEN);
	QRect canvasRect = rect();
	painter.drawRect(canvasRect);
	painter.drawPixmap(2, 2, canvasPixmap);
}

void CanvasPaint::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (canvasPixmap.rect().contains(event->pos()))
		{
			lastPoint = event->pos();
		}
	}
}

void CanvasPaint::mouseMoveEvent(QMouseEvent* event)
{
	if (canvasPixmap.rect().contains(event->pos()))
	{
		QPoint currentPos = event->pos();

		if (drawState == DrawingState::DRAWING)
		{
			QPainter painter(&canvasPixmap);
			painter.setPen(DRAWING_PEN);
			currentLine.points.append(currentPos);

			for (int i = 1; i < currentLine.points.size(); i++)
			{
				painter.drawLine(currentLine.points[i - 1], currentLine.points[i]);
			}

			update();
		}

		else if (drawState == DrawingState::ERASING)
		{
			QPainter painter(&canvasPixmap);
			painter.setCompositionMode(QPainter::CompositionMode_Source);
			painter.setPen(ERASING_PEN);

			currentLine.points.append(currentPos);

			for (int i = 1; i < currentLine.points.size(); i++)
			{
				painter.drawLine(currentLine.points[i - 1], currentLine.points[i]);
			}

			update();
		}
	}
}

#ifdef ONLINE
std::vector<common::img::Point> CanvasPaint::convertToCommonPoints(const DrawnLine& line)
{
	std::vector<common::img::Point> commonPoints;
	commonPoints.reserve(line.points.size());
	for (const auto& point : line.points)
	{
		common::img::Point commonPoint;
		commonPoint.x = point.x();
		commonPoint.y = point.y();
		commonPoint.color = (line.drawState == DrawingState::DRAWING ? 0x000000 : 0xFFFFFF);
		commonPoints.emplace_back(std::move(commonPoint));
	}
	return commonPoints;
}
#endif

void CanvasPaint::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		currentLine.drawState = drawState;
		drawnLines.append(currentLine);

#ifdef ONLINE
		services::SendImageUpdates(0, convertToCommonPoints(currentLine));
#endif

		currentLine.points.clear();
	}
}

void CanvasPaint::resizeEvent(QResizeEvent* event)
{
	QPixmap newPixmap(event->size().width() * 3 / 4, event->size().height());
	newPixmap.fill(Qt::white);
	QPainter painter(&newPixmap);
	painter.drawPixmap(QRect(0, 0, event->size().width() * 3 / 4, event->size().height()), canvasPixmap);
	canvasPixmap = std::move(newPixmap);

	update();
}

void CanvasPaint::clearCanvas()
{
	canvasPixmap.fill(Qt::white);
	drawnLines.clear();
	update();
}

void CanvasPaint::on_leaveServerButton_clicked()
{
	hide();
	signInWindow = new MainWindow(this);
	signInWindow->show();
}

void CanvasPaint::on_resetCanvas_clicked()
{
	clearCanvas();
	drawState = DrawingState::DRAWING;
}

void CanvasPaint::on_drawButton_clicked()
{
	drawState = DrawingState::DRAWING;
}

void CanvasPaint::on_eraseButton_clicked()
{
	drawState = DrawingState::ERASING;
}

void CanvasPaint::on_undoButton_clicked()
{
	if (!drawnLines.isEmpty())
	{
		canvasPixmap.fill(Qt::white);
		QPainter painter(&canvasPixmap);
		drawnLines.pop_back();

		for (const auto& line : drawnLines)
		{
			painter.setPen(line.drawState == DrawingState::DRAWING ? DRAWING_PEN : ERASING_PEN);

			for (int i = 1; i < line.points.size(); i++)
				painter.drawLine(line.points[i - 1], line.points[i]);
		}

		update();
	}
}

void CanvasPaint::on_messageButton_clicked()
{
	ui->messageBox->clear();
}

void CanvasPaint::closeEvent(QCloseEvent* event)
{
	QCoreApplication::quit();
	event->accept();
}
