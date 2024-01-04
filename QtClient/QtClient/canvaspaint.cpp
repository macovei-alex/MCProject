#include "canvaspaint.h"
#include "mainwindow.h"
#include "ui_canvaspaint.h"
#include <QGuiApplication>
#include <QScreen>

CanvasPaint::CanvasPaint(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::CanvasPaint),
	drawState{ DrawState::DRAWING }
{
	ui->setupUi(this);

	ui->gameChatLabel->setStyleSheet("border: none;");
	ui->gameChat->setStyleSheet("QWidget { border: 1px solid black; }");

	QScreen* primaryScreen = QGuiApplication::primaryScreen();
	QSize screenSize = primaryScreen->geometry().size();
	setGeometry(0, 0, screenSize.width(), screenSize.height());
	setStyleSheet("QDialog { border: 2px solid black; }");

	canvasPixmap = QPixmap(screenSize.width() * 3 / 4, screenSize.height()); // Ajustează dimensiunile după necesități
	canvasPixmap.fill(Qt::white); // Umple canvas-ul cu culoarea albă

	setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
}

CanvasPaint::~CanvasPaint()
{
	delete ui;
}

void CanvasPaint::setDrawState(DrawState state)
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

// Actualizează metoda mousePressEvent
void CanvasPaint::mousePressEvent(QMouseEvent* event)
{
	// Verifică dacă evenimentul este un clic al mouse-ului
	if (event->button() == Qt::LeftButton)
	{
		if (event->x() < width() * 3 / 4)
		{
			lastPoint = event->pos();
		}
	}
}

// Adaugă o nouă metodă pentru gestionarea mișcării mouse-ului
void CanvasPaint::mouseMoveEvent(QMouseEvent* event)
{
	if (canvasPixmap.rect().contains(event->pos()))
	{
		QPoint currentPos = event->pos();

		if (drawState == DrawState::DRAWING)
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

		else if (drawState == DrawState::ERASING)
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

void CanvasPaint::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		currentLine.drawState = drawState;
		drawnLines.append(currentLine);
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
	drawState = DrawState::DRAWING;
}

void CanvasPaint::on_drawButton_clicked()
{
	drawState = DrawState::DRAWING;
}

void CanvasPaint::on_eraseButton_clicked()
{
	drawState = DrawState::ERASING;
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
			painter.setPen(line.drawState == DrawState::DRAWING ? DRAWING_PEN : ERASING_PEN);

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
