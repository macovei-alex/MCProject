#include "mainwindow.h"
#include "canvaspaint.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QPixmap>

#ifdef ONLINE
#endif

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow{ parent },
	ui{ new Ui::MainWindow },
	roomID{ 0 }
{
	ui->setupUi(this);

	QPixmap backgroundImage{ ":Resource Files/Images/Background.jpg" };

#ifdef ONLINE
	auto [width, height] { ui->centralwidget->size() };
#else
	int width = ui->centralwidget->width();
	int height = ui->centralwidget->height();
#endif

	ui->backgroundLabel->setPixmap(backgroundImage.scaled(width, height, Qt::KeepAspectRatio));

	QPixmap humanFigure{ ":Resource Files/Images/login_icon.png" };
    ui->humanFigureLabel->setPixmap(humanFigure.scaled(100, 100, Qt::KeepAspectRatio));
    ui->humanFigureLabel->raise();

	/* QQuickWidget *qmlWidget = new QQuickWidget();
	 qmlWidget->setSource(QUrl("qrc:/Canvas.qml"));
	 ui->centralwidget->layout()->addWidget(qmlWidget);
	 QObject:: connect(qmlWidget->rootObject(), SIGNAL(qmlSignal()), this, SLOT(handelQmlSignal()));*/
}


MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_loginButton_clicked()
{
	if (ui->usernameLineEdit->text().isEmpty() || ui->passwordLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Sign up / Sign in", "Username or password is empty");
		return;
	}

#ifdef ONLINE
	if (!services::SignIn(
		ui->usernameLineEdit->text().toStdString(),
		ui->passwordLineEdit->text().toStdString()))
	{
		QMessageBox msgBox;
		msgBox.setText("No account with username " + ui->usernameLineEdit->text() +
			"and password " + ui->passwordLineEdit->text() +
			" exists. Do you want to create one?");

		QPushButton* yesButton{ msgBox.addButton(tr("Yes"), QMessageBox::YesRole) };
		QPushButton* noButton{ msgBox.addButton(tr("No"), QMessageBox::NoRole) };
		msgBox.setDefaultButton(noButton);

		msgBox.exec();

		if (msgBox.clickedButton() == yesButton)
		{
			if (!services::SignUp(
				ui->usernameLineEdit->text().toStdString(),
				ui->passwordLineEdit->text().toStdString()))
			{
				QMessageBox::warning(this, "Sign up", "Could not cerate a new account");
				return;
			}
		}
		else
		{
			return;
		}
	}

	hide();
	// roomID = services::CreateRoom();
	roomID = 0;
	canvasPaint = new CanvasPaint(roomID, this);
#else
	canvasPaint = new CanvasPaint(this);
#endif

	canvasPaint->show();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	QCoreApplication::quit();
	event->accept();
}
