#include "canvaspaint.h"
#include "mainwindow.h"
#include "ui_canvaspaint.h"

#include <QGuiApplication>
#include <QScreen>
#include <QObject>

const uint32_t DrawnLine::DRAWING_COLOR_INT = 0x000000;
const uint32_t DrawnLine::ERASING_COLOR_INT = 0xFFFFFF;

#ifdef ONLINE
DrawnLine::DrawnLine(std::vector<common::img::Point>&& commonPoints, uint32_t color) :
	drawState{ color == ERASING_COLOR_INT ? DrawingState::ERASING : DrawingState::DRAWING }
{
	points.reserve(commonPoints.size());
	for (const auto& commonPoint : commonPoints)
		points.emplace_back(commonPoint.x, commonPoint.y);
}

std::vector<common::img::Point> DrawnLine::ToCommonPoints() const
{
	std::vector<common::img::Point> commonPoints;
	commonPoints.reserve(points.size());

	for (const auto& point : points)
	{
		commonPoints.emplace_back(
			point.x(),
			point.y(),
			(drawState == DrawingState::DRAWING ? DRAWING_COLOR_INT : ERASING_COLOR_INT));
	}

	return commonPoints;
}

ImageReceiver::ImageReceiver(uint64_t roomID, bool& keepGoing, QWidget* parent) :
	QThread(parent),
	keepGoing{ keepGoing },
	roomID{ roomID }
{
	/* empty */
}

void ImageReceiver::run()
{
	using std::chrono_literals::operator""s;
	try
	{
		while (keepGoing)
		{
			auto points{ std::move(services::ReceiveImageUpdates(roomID)) };
			qDebug() << "Received points: " << points.size();

			if (points.size() != 0)
			{
				QList<DrawnLine>* newLines = new QList<DrawnLine>();
				DrawnLine line;
				line.drawState = (points[0].color == DrawnLine::ERASING_COLOR_INT ? DrawingState::ERASING : DrawingState::DRAWING);

				for (const auto& point : points)
				{
					uint32_t currentColor = (line.drawState == DrawingState::DRAWING ? DrawnLine::DRAWING_COLOR_INT : DrawnLine::ERASING_COLOR_INT);
					if (point.color != currentColor)
					{
						if (line.points.size() > 0)
						{
							newLines->append(line);
						}

						line.points.clear();
						line.drawState = (point.color == DrawnLine::ERASING_COLOR_INT ? DrawingState::ERASING : DrawingState::DRAWING);
					}

					qDebug() << "Point: " << point.x << ", " << point.y << ", " << point.color.ToInt32();
					line.points.append(QPoint(point.x, point.y));
				}

				qDebug() << "Received lines: " << newLines->size();

				emit LinesReceived(newLines);
			}

			std::this_thread::sleep_for(0.25s);
		}
	}
	catch (const std::exception& e)
	{
		qDebug() << e.what() << '\n';
	}
}
#endif

CanvasPaint::CanvasPaint(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::CanvasPaint),
	drawState{ DrawingState::DRAWING },
	drawnLines{ }
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

	drawState = DrawingState::DRAWING;

	setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
}

#ifdef ONLINE
CanvasPaint::CanvasPaint(uint64_t roomID, QWidget* parent) :
	QDialog(parent),
	ui(new Ui::CanvasPaint),
	drawState{ DrawingState::DRAWING },
	keepGoing{ true },
	roomID{ roomID },
	drawnLines{ }
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

	drawState = DrawingState::DRAWING;

	setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);

	imageReceiver = new	ImageReceiver(roomID, keepGoing, this);
	connect(imageReceiver, &ImageReceiver::LinesReceived, this, &CanvasPaint::HandleAddLines);
	connect(imageReceiver, &ImageReceiver::finished, imageReceiver, &QObject::deleteLater);
	imageReceiver->start();
}
#endif

CanvasPaint::~CanvasPaint()
{

#ifdef ONLINE
	keepGoing = false;
	imageReceiver->quit();
	imageReceiver->wait();
#endif

	delete ui;
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

void CanvasPaint::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		currentLine.drawState = drawState;
		drawnLines.append(currentLine);

		#ifdef ONLINE
		auto commonPoints = currentLine.ToCommonPoints();
		qDebug() << "Sending points: " << commonPoints.size();
		if (commonPoints.size() != 0)
		{
			qDebug() << "First point: " << commonPoints[0].color.ToInt32();
		}
		services::SendImageUpdates(roomID, currentLine.ToCommonPoints());
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

void CanvasPaint::ClearCanvas()
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
	ClearCanvas();
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

void CanvasPaint::HandleAddLines(QList<DrawnLine>* newLines)
{
	QPainter painter(&canvasPixmap);

	for (auto& line : *newLines)
	{
		painter.setPen(line.drawState == DrawingState::DRAWING ? DRAWING_PEN : ERASING_PEN);
		for (int i = 1; i < line.points.size(); i++)
			painter.drawLine(line.points[i - 1], line.points[i]);

		drawnLines.append(line);
	}

	delete newLines;
	update();
}

void CanvasPaint::closeEvent(QCloseEvent* event)
{
	QCoreApplication::quit();
	event->accept();
}
