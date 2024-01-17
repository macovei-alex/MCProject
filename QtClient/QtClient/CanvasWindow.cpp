#include "CanvasWindow.h"
#include "MainWindow.h"
#include "ui_CanvasWindow.h"
#include "ChooseWordWindow.h"

#include <QGuiApplication>
#include <QScreen>
#include <QObject>

#include "common.h"

#ifdef ONLINE
#include "services.h"
#endif

CanvasWindow::CanvasWindow(QWidget* parent) :
	QDialog{ parent },
	ui{ new Ui::CanvasWindow },
	m_drawState{ DrawingState::DRAWING }
{
	ui->setupUi(this);

	ui->roomLabel->setText("Room ID: none");
	ui->gameChatLabel->setStyleSheet("border: none;");

	QSize screenSize{ QGuiApplication::primaryScreen()->geometry().size() };
	setGeometry(0, 0, screenSize.width(), screenSize.height());
	setStyleSheet("QDialog { border: 2px solid black; }");

	canvasPixmap = QPixmap{ screenSize.width() * 3 / 4, screenSize.height() };
	canvasPixmap.fill(Qt::white);

	setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

#ifndef ONLINE
	ui->startGameButton->hide();
	ui->chosenWord->hide();
	ui->playerScore->hide();
	ui->timerLabel->hide();
	ui->roomLabel->hide();
#endif

}

#ifdef ONLINE
CanvasWindow::CanvasWindow(uint64_t roomID, const QString& username, QWidget* parent) :
	QDialog{ parent },
	ui{ new Ui::CanvasWindow },
	m_drawState{ DrawingState::DRAWING },
	m_keepGoing{ true },
	m_onlineData{ roomID, username },
	m_imageThread{ std::make_unique<ImageThread>(roomID, m_keepGoing, this) },
	m_gameStateThread{ std::make_unique<GameStateThread>(roomID, m_keepGoing, this) },
	m_chatThread{ std::make_unique<ChatThread>(roomID, username, m_keepGoing, this) }
{
	ui->setupUi(this);

	ui->roomLabel->setText("Room ID: " + QString::number(static_cast<qint64>(roomID)));
	ui->roomLabel->update();
	ui->gameChatLabel->setStyleSheet("border: none;");
	//ui->gameChatLabel->setStyleSheet("QWidget { border: 1px solid black; }");

	QSize screenSize{ QGuiApplication::primaryScreen()->geometry().size() };
	setGeometry(0, 0, screenSize.width(), screenSize.height());
	setStyleSheet("QDialog { border: 2px solid black; }");

	canvasPixmap = QPixmap{ screenSize.width() * 3 / 4, screenSize.height() };
	canvasPixmap.fill(Qt::white);

	setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

	connect(&*m_imageThread, &ImageThread::ImageSignal, this, &CanvasWindow::HandleImage);
	connect(&*m_imageThread, &ImageThread::finished, &*m_imageThread, &QObject::deleteLater);

	connect(&*m_gameStateThread, &GameStateThread::GameStateSignal, this, &CanvasWindow::HandleGameState);
	connect(&*m_gameStateThread, &GameStateThread::finished, &*m_gameStateThread, &QObject::deleteLater);

	connect(&*m_chatThread, &ChatThread::ChatSignal, this, &CanvasWindow::HandleChat);
	connect(&*m_chatThread, &ChatThread::finished, &*m_chatThread, &QObject::deleteLater);

	m_imageThread->start();
	m_gameStateThread->start();
	m_chatThread->start();

	/*m_imageThread->Pause();
	m_chatThread->Pause();*/
}
#endif

CanvasWindow::~CanvasWindow()
{
	delete ui;
}

void CanvasWindow::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	QPainter painter(this);
	painter.setPen(kDRAWING_PEN);

	QRect canvasRect = rect();
	painter.drawRect(canvasRect);
	painter.drawPixmap(2, 2, canvasPixmap);
}

void CanvasWindow::mousePressEvent(QMouseEvent* event)
{
	if (event->button() != Qt::LeftButton)
		return;

#ifdef ONLINE
	if (m_onlineData.playerRole != common::game::PlayerRole::DRAWING)
		return;
#endif

	if (event->button() == Qt::LeftButton)
	{
		if (canvasPixmap.rect().contains(event->pos()))
		{
			lastPoint = event->pos();
		}
	}
}

void CanvasWindow::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() != Qt::LeftButton)
		return;

	if (!canvasPixmap.rect().contains(event->pos()))
		return;

#ifdef ONLINE
	if (m_onlineData.playerRole != common::game::PlayerRole::DRAWING)
		return;
#endif

	QPoint currentPos{ event->pos() };

	if (m_drawState == DrawingState::DRAWING)
	{
		QPainter painter{ &canvasPixmap };
		painter.setPen(kDRAWING_PEN);

		currentLine.points.emplace_back(currentPos);

		if (currentLine.points.size() > 1)
		{
			painter.drawLine(
				currentLine.points[currentLine.points.size() - 2],
				currentLine.points[currentLine.points.size() - 1]);
		}

		update();
	}

	else if (m_drawState == DrawingState::ERASING)
	{
		QPainter painter{ &canvasPixmap };
		painter.setCompositionMode(QPainter::CompositionMode_Source);
		painter.setPen(kERASING_PEN);

		currentLine.points.emplace_back(currentPos);

		if (currentLine.points.size() > 1)
		{
			painter.drawLine(
				currentLine.points[currentLine.points.size() - 2],
				currentLine.points[currentLine.points.size() - 1]);
		}

		update();
	}
}

void CanvasWindow::mouseReleaseEvent(QMouseEvent* event)
{

#ifdef ONLINE
	if (m_onlineData.playerRole != common::game::PlayerRole::DRAWING)
		return;
#endif

	if (event->button() == Qt::LeftButton)
	{
		currentLine.drawState = m_drawState;

#ifdef ONLINE
		if (currentLine.points.size() > 1)
		{
			auto commonPoints{ std::move(currentLine.ToCommonPoints()) };
			commonPoints.emplace_back(common::img::Point{ -1, -1, Line::INVALID_COLOR_INT });
			services::SendImageUpdates(m_onlineData.roomID, commonPoints);
		}
#endif

		lines.emplace_back(std::move(currentLine));
	}
}

void CanvasWindow::resizeEvent(QResizeEvent* event)
{
	QPixmap newPixmap{ event->size().width() * 3 / 4, event->size().height() };
	newPixmap.fill(Qt::white);

	QPainter painter{ &newPixmap };
	painter.drawPixmap(QRect{ 0, 0, event->size().width() * 3 / 4, event->size().height() }, canvasPixmap);
	canvasPixmap = std::move(newPixmap);

	update();
}

void CanvasWindow::ClearCanvas()
{
	/*canvasPixmap.fill(Qt::white);
	lines.clear();
	update();*/
}

void CanvasWindow::on_resetCanvas_clicked()
{
	/*ClearCanvas();
	m_drawState = DrawingState::DRAWING;*/
}

void CanvasWindow::on_drawButton_clicked()
{
	m_drawState = DrawingState::DRAWING;
}

void CanvasWindow::on_eraseButton_clicked()
{
	m_drawState = DrawingState::ERASING;
}

void CanvasWindow::on_undoButton_clicked()
{
	/*if (!lines.isEmpty())
	{
		canvasPixmap.fill(Qt::white);
		QPainter painter{ &canvasPixmap };
		lines.pop_back();

		for (const auto& line : lines)
		{
			painter.setPen(line.drawState == DrawingState::DRAWING ? kDRAWING_PEN : kERASING_PEN);

			for (qsizetype i = 1; i < line.points.size(); i++)
				painter.drawLine(line.points[i - 1], line.points[i]);
		}

		update();
	}*/
}

void CanvasWindow::on_messageButton_clicked()
{

#ifdef ONLINE
	QString formattedMessage{ QString{"[%1]: %2"}.arg(m_onlineData.username).arg(ui->messageBox->text()) };

	ui->gameChatLabel->setText(ui->gameChatLabel->text() + "\n" + formattedMessage);

	services::SendNewMessage(m_onlineData.username.toStdString(),
		ui->messageBox->text().toStdString(), m_onlineData.roomID);
	ui->messageBox->clear();
#endif

}

void CanvasWindow::on_startGameButton_clicked()
{

#ifdef ONLINE
	services::StartGame(m_onlineData.roomID);
	SetAllThreadsPauseStatus(false);
#endif

}

void CanvasWindow::closeEvent(QCloseEvent* event)
{

#ifdef ONLINE
	services::SignOut(m_onlineData.username.toStdString());
	m_keepGoing = false;

	m_imageThread->quit();
	m_gameStateThread->quit();
	m_chatThread->quit();

	m_imageThread->wait();
	m_gameStateThread->wait();
	m_chatThread->wait();

	m_imageThread;
	m_gameStateThread;
	m_chatThread;
#endif

	parentWidget()->show();
	event->accept();
}

void CanvasWindow::HandleImage(QList<Line>* newLines)
{

#ifdef ONLINE
	QPainter painter{ &canvasPixmap };

	for (auto& line : *newLines)
	{
		painter.setPen((line.drawState == DrawingState::DRAWING ? kDRAWING_PEN : kERASING_PEN));
		for (qsizetype i = 1; i < line.points.size(); i++)
			painter.drawLine(line.points[i - 1], line.points[i]);

		lines.emplace_back(std::move(line));
	}

	delete newLines;
	update();
#endif

}

void CanvasWindow::HandleGameState(const QPair<common::game::GameState, uint64_t>& gameStatePair)
{

#ifdef ONLINE
	auto lastGameState{ m_onlineData.gameState };
	m_onlineData.gameState = gameStatePair.first;
	m_onlineData.playerRole = services::ReceivePlayerRole(m_onlineData.roomID, m_onlineData.username.toStdString());

	ui->timerLabel->setText("Time left: " + QString::number(gameStatePair.second));

	if (m_onlineData.gameState == common::game::GameState::PICK_WORD)
	{
		if (lastGameState != common::game::GameState::PICK_WORD)
		{
			ClearCanvas();
			auto scores{ services::ReceivePlayerScores(m_onlineData.roomID) };
			for(const auto& [name, score]: scores)
				if (name == m_onlineData.username.toStdString())
				{
					ui->playerScore->setText("Score: " + QString::number(score));
					break;
				}

			if (m_onlineData.playerRole == common::game::PlayerRole::GUESSING)
			{
				m_onlineData.chosenWord = "";
				ui->chosenWord->hide();
				return;
			}
		}

		if (m_onlineData.playerRole != common::game::PlayerRole::DRAWING
			|| lastGameState == m_onlineData.gameState)
		{
			return;
		}

		auto words{ services::ReceiveWordOptions(m_onlineData.roomID) };
		std::unique_ptr<ChooseWordWindow> chooseWordWindow{ std::make_unique<ChooseWordWindow>(this) };
		chooseWordWindow->setButtonNames(words);

		if (chooseWordWindow->exec() == QDialog::Accepted)
		{
			services::SendGuessingWord(m_onlineData.roomID, m_onlineData.chosenWord.toStdString());
			ui->chosenWord->setText("Chosen Word: " + m_onlineData.chosenWord);
			ui->chosenWord->show();

			m_chatThread->Unpause();
		}
	}

	else if (m_onlineData.gameState == common::game::GameState::DRAW_AND_GUESS)
	{
		if (m_onlineData.playerRole == common::game::PlayerRole::DRAWING)
		{
			/*m_imageThread->Pause();*/
		}
		else if (m_onlineData.playerRole == common::game::PlayerRole::GUESSING)
		{
			if (lastGameState != common::game::GameState::DRAW_AND_GUESS)
			{
				m_chatThread->Unpause();
			}
		}
	}

	else if (m_onlineData.gameState == common::game::GameState::NONE)
	{
		m_onlineData.chosenWord = "";
		/*m_chatThread->Pause();
		m_imageThread->Pause();*/
	}
#endif

}

void CanvasWindow::HandleChat(const QList<common::Message>& messages)
{

#ifdef ONLINE
	for (const auto& message : messages)
	{
		QString formattedMessage{ '[' + QString::fromStdString(message.author) + "]: " + QString::fromStdString(message.text) };

		qDebug() << "Recieved message: " << formattedMessage << "\n";
		ui->gameChatLabel->setText(ui->gameChatLabel->text() + "\n" + formattedMessage);
	
	}
	ui->gameChatLabel->update();
#endif

}

#ifdef ONLINE
void CanvasWindow::SetAllThreadsPauseStatus(bool paused)
{

	if (paused == true)
	{
		m_imageThread->Pause();
		m_gameStateThread->Pause();
		m_chatThread->Pause();
	}
	else
	{
		m_imageThread->Unpause();
		m_gameStateThread->Unpause();
		m_chatThread->Unpause();
	}
}
#endif

void CanvasWindow::SetAllButtonsEnabled(bool enabled)
{
	ui->startGameButton->setEnabled(enabled);
	ui->resetCanvas->setEnabled(enabled);
	ui->drawButton->setEnabled(enabled);
	ui->eraseButton->setEnabled(enabled);
	ui->undoButton->setEnabled(enabled);
	ui->messageButton->setEnabled(enabled);
	ui->messageBox->setEnabled(enabled);
}

#ifdef ONLINE
const OnlineData& CanvasWindow::GetOnlineData()
{
	return m_onlineData;
}
#endif

#ifdef ONLINE
void CanvasWindow::SetChosenWord(const QString& word)
{
	m_onlineData.chosenWord = word;
}
#endif
