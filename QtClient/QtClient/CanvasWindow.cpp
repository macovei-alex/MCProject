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
	//ui->gameChat->setStyleSheet("QWidget { border: 1px solid black; }");

	QSize screenSize{ QGuiApplication::primaryScreen()->geometry().size() };
	setGeometry(0, 0, screenSize.width(), screenSize.height());
	setStyleSheet("QDialog { border: 2px solid black; }");

	canvasPixmap = QPixmap{ screenSize.width() * 3 / 4, screenSize.height() };
	canvasPixmap.fill(Qt::white);

	setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);

	roomLabel = new QLabel(this);
	roomLabel->setText(QString{ "Room ID: " } + QString::number(static_cast<qint64>(roomID)));
	chosedWord = new QLabel(this);
	chosedWord->setText("Chosen Word: none");
	chosedWord->move(10, 10); 
	chosedWord->show();
}


void CanvasWindow::setRoomID(uint64_t roomID)
{
	this->roomID = roomID;
	ui->roomLabel->setText(QString{ "Room ID: " } + QString::number(static_cast<qint64>(roomID)));
	ui->roomLabel->update();
}

#ifdef ONLINE
CanvasWindow::CanvasWindow(uint64_t roomID, const QString& username, QWidget* parent) :
	QDialog{ parent },
	ui{ new Ui::CanvasWindow },
	m_drawState{ DrawingState::DRAWING },
	m_keepGoing{ true },
	m_onlineData{ roomID, username },
	m_imageThread{ new ImageThread(roomID, m_keepGoing, this) },
	m_gameStateThread{ new GameStateThread(roomID, m_keepGoing, this) },
	m_chatThread{ new ChatThread(roomID, username, m_keepGoing, this) }
{
	ui->setupUi(this);

	ui->roomLabel->setText(QString{ "Room ID: " } + QString::number(static_cast<qint64>(roomID)));
	ui->roomLabel->update();
	ui->gameChatLabel->setStyleSheet("border: none;");
	//ui->gameChat->setStyleSheet("QWidget { border: 1px solid black; }");

	QSize screenSize{ QGuiApplication::primaryScreen()->geometry().size() };
	setGeometry(0, 0, screenSize.width(), screenSize.height());
	setStyleSheet("QDialog { border: 2px solid black; }");

	canvasPixmap = QPixmap{ screenSize.width() * 3 / 4, screenSize.height() };
	canvasPixmap.fill(Qt::white);

	setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);

	connect(m_imageThread, &ImageThread::ImageSignal, this, &CanvasWindow::HandleImage);
	connect(m_imageThread, &ImageThread::finished, m_imageThread, &QObject::deleteLater);

	connect(m_gameStateThread, &GameStateThread::GameStateSignal, this, &CanvasWindow::HandleGameState);
	connect(m_gameStateThread, &GameStateThread::finished, m_gameStateThread, &QObject::deleteLater);

	connect(m_chatThread, &ChatThread::ChatSignal, this, &CanvasWindow::HandleChat);
	connect(m_chatThread, &ChatThread::finished, m_chatThread, &QObject::deleteLater);

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

#ifdef ONLINE
	if (m_onlineData.playerRole != common::game::PlayerRole::DRAWING)
		return;
#endif

	if (!canvasPixmap.rect().contains(event->pos()))
		return;

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
	canvasPixmap.fill(Qt::white);
	lines.clear();
	update();
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
	QString formattedMessage{ QString{"[%1]: %2"}.arg(m_onlineData.username).arg(ui->messageBox->text()) };

	ui->gameChatLabel->setText(ui->gameChatLabel->text() + "\n" + formattedMessage);

#ifdef ONLINE
	services::SendNewMessage(m_onlineData.username.toStdString(),
		ui->messageBox->text().toStdString(), m_onlineData.roomID);
#endif

	ui->messageBox->clear();
}

void CanvasWindow::on_startGameButton_clicked()
{
	services::StartGame(m_onlineData.roomID);
	SetAllThreadsPauseStatus(false);
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

	delete m_imageThread;
	delete m_gameStateThread;
	delete m_chatThread;
#endif

	parentWidget()->show();
	event->accept();
}

#ifdef ONLINE
void CanvasWindow::HandleImage(QList<Line>* newLines)
{
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
}

void CanvasWindow::HandleGameState(const QPair<common::game::GameState, uint64_t>& gameStatePair)
{
	auto lastGameState{ m_onlineData.gameState };
	m_onlineData.gameState = gameStatePair.first;
	m_onlineData.playerRole = services::ReceivePlayerRole(m_onlineData.roomID, m_onlineData.username.toStdString());

	ui->timerLabel->setText(QString::number(gameStatePair.second));

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
		}

		if (m_onlineData.playerRole != common::game::PlayerRole::DRAWING
			|| lastGameState == m_onlineData.gameState)
		{
			return;
		}

		auto words{ services::ReceiveWordOptions(m_onlineData.roomID) };
		ChooseWordWindow* chooseWordWindow{ new ChooseWordWindow(this) };
		chooseWordWindow->setButtonNames(words);

		if (chooseWordWindow->exec() == QDialog::Accepted)
		{ 
			services::SendGuessingWord(m_onlineData.roomID, m_onlineData.chosenWord.toStdString());
			m_chatThread->Unpause();
		}

		delete chooseWordWindow;
	}

	else if (m_onlineData.gameState == common::game::GameState::DRAW_AND_GUESS)
	{
		if (m_onlineData.playerRole == common::game::PlayerRole::DRAWING)
		{
			m_drawState = DrawingState::DRAWING;
		}
		else if (m_onlineData.playerRole == common::game::PlayerRole::GUESSING)
		{
			m_onlineData.chosenWord = "";
			m_chatThread->Unpause();
		}
	}

	else if (m_onlineData.gameState == common::game::GameState::NONE)
	{
		m_onlineData.chosenWord = "";
		/*m_chatThread->Pause();
		m_imageThread->Pause();*/
	}
}


void CanvasWindow::HandleChat(const QList<common::Message>& messages)
{ 
	for (const auto& message : messages)
	{
		QString formattedMessage{ '[' + QString::fromStdString(message.author) + "]: " + QString::fromStdString(message.text) };

		qDebug() << " Recieved message: " << formattedMessage << "\n";
		ui->gameChatLabel->setText(ui->gameChatLabel->text() + "\n" + formattedMessage);
	
	}
	ui->gameChatLabel->update();
}

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

void CanvasWindow::updatePlayerScoreLabel(const std::vector<std::pair<std::string, int32_t>>& scores)
{
	QString labelText;
	for (const auto& playerScore : scores)
	{
		labelText += QString("%1: %2\n")
			.arg(playerScore.first.c_str())
			.arg(playerScore.second);
	}

	ui->playerScore->setText("Scores: " + labelText);
}
void CanvasWindow::SetAllButtonsEnabled(bool enabled)
{
	ui->startGameButton->setEnabled(enabled);
	ui->resetCanvas->setEnabled(enabled);
	ui->drawButton->setEnabled(enabled);
	ui->eraseButton->setEnabled(enabled);
	ui->undoButton->setEnabled(enabled);
	ui->messageButton->setEnabled(enabled);
	ui->messageBox->setEnabled(enabled);
	//ui->gameChat->setEnabled(enabled);
}

const OnlineData& CanvasWindow::GetOnlineData()
{
	return m_onlineData;
}

void CanvasWindow::SetChosenWord(const QString& word)
{
	m_onlineData.chosenWord = word;
}
#endif
