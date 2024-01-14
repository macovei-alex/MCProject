#include "mainwindow.h"
#include "CanvasWindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QPixmap>

#ifdef ONLINE
#include "services.h"
#endif

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow{ parent },
	ui{ new Ui::MainWindow },
	roomID{ },
	m_isConnected{ false }
{
	ui->setupUi(this);

	QPixmap backgroundImage{ ":Resource Files/Images/Background.jpg" };
	ui->backgroundLabel->setPixmap(backgroundImage.scaled(ui->centralwidget->size(), Qt::KeepAspectRatio));
	ui->backgroundLabel->raise();

	QPixmap humanFigure{ ":Resource Files/Images/login_icon.png" };
	ui->humanFigureLabel->setPixmap(humanFigure.scaled(100, 100, Qt::KeepAspectRatio));
	ui->humanFigureLabel->raise();
}


MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_loginButton_clicked()
{
	static QString username{ "" };

	if (ui->usernameLineEdit->text().isEmpty() || ui->passwordLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Sign up / Sign in", "Username or password is empty");
		return;
	}

#ifdef ONLINE
	if (m_isConnected)
	{
		QMessageBox msgBox;
		msgBox.setText("You are already connected " + username +
			"Do you want to disconnect from this account and sign in as " + ui->usernameLineEdit->text() + "?");

		QPushButton* yesButton{ msgBox.addButton(tr("Yes"), QMessageBox::YesRole) };
		QPushButton* noButton{ msgBox.addButton(tr("No"), QMessageBox::NoRole) };
		msgBox.setDefaultButton(noButton);

		msgBox.exec();

		if (msgBox.clickedButton() == yesButton)
		{
			if(!services::SignOut(username.toStdString()))
				QMessageBox::warning(this, "Sign out", "Could not sign out");
			else
				m_isConnected = false;
		}
		else
			return;
	}

	if (services::SignIn(
		ui->usernameLineEdit->text().toStdString(),
		ui->passwordLineEdit->text().toStdString()))
	{
		username = ui->usernameLineEdit->text();
		m_isConnected = true;
		return;
	}

	QMessageBox msgBox;
	msgBox.setText("No account with username " + ui->usernameLineEdit->text() +
		" and password " + ui->passwordLineEdit->text() +
		" exists or the someone is already connected with this account. " +
		"Do you want to try to create one ? ");

	QPushButton* yesButton{ msgBox.addButton(tr("Yes"), QMessageBox::YesRole) };
	QPushButton* noButton{ msgBox.addButton(tr("No"), QMessageBox::NoRole) };
	msgBox.setDefaultButton(noButton);

	msgBox.exec();

	if (msgBox.clickedButton() != yesButton)
		return;

	if (services::SignUp(
		ui->usernameLineEdit->text().toStdString(),
		ui->passwordLineEdit->text().toStdString()))
	{
		m_isConnected = true;
		return;
	}

	QMessageBox::warning(this, "Sign up", "Could not cerate a new account");
#endif
}

void MainWindow::on_joinRoomButton_clicked()
{
#ifdef ONLINE
	if (!m_isConnected)
	{
		QMessageBox::warning(this, "Join room", "You are not connected");
		return;
	}

	try
	{
		QString numberStr{ ui->joinRoomLineEdit->text() };

		if (numberStr.isEmpty())
			throw std::exception{};

		roomID = static_cast<uint64_t>(numberStr.toULongLong());
	}
	catch (...)
	{
		QMessageBox::warning(this, "Join room", "The text cannot be converted to a number");
		return;
	}

	if (!services::ConnectToRoom(roomID, ui->usernameLineEdit->text().toStdString()))
	{
		QMessageBox::warning(this, "Join room", "Could not connect to the room");
		return;
	}

	canvasWindow = new CanvasWindow(roomID, ui->usernameLineEdit->text(), this);
#else
	canvasWindow = new CanvasWindow(this);
#endif

	hide();

	canvasWindow->show();
	}

void MainWindow::on_createRoomButton_clicked()
{
#ifdef ONLINE
	if (!m_isConnected)
	{
		QMessageBox::warning(this, "Create room", "You are not connected");
		return;
	}

	try
	{
		roomID = services::CreateRoom(ui->usernameLineEdit->text().toStdString());
		if (roomID == MAXUINT64)
			throw std::exception{};
	}
	catch (...)
	{
		QMessageBox::warning(this, "Create room", "Could not create a new room (1)");
		return;
	}

	hide();
	canvasWindow = new CanvasWindow(roomID, ui->usernameLineEdit->text(), this);

#else
	canvasWindow = new CanvasWindow(this);
#endif

	canvasWindow->show();
}

void MainWindow::closeEvent(QCloseEvent* event)
{

#ifdef ONLINE
	if (m_isConnected)
		services::SignOut(ui->usernameLineEdit->text().toStdString());
#endif

	QCoreApplication::quit();
	event->accept();
}
