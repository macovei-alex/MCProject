#include "mainwindow.h"
#include "canvaspaint.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QPixmap>

#define DO_SERVER_CONNECT 0

#if defined(_MSVC_LANG) && (_MSVC_LANG == 202002L) && (DO_SERVER_CONNECT == 1)

#include "services.h"

#endif

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QPixmap pix(":Resource Files/Images/Background.jpg");
    int width = ui->backgroundLabel->width();
    int height = ui->backgroundLabel->height();
    ui->backgroundLabel->setPixmap(pix.scaled(width, height, Qt::KeepAspectRatio));
	QPixmap pix1(":Resource Files/Images/login_icon.png");
    ui->loginLabel->setPixmap(pix1.scaled(100, 100, Qt::KeepAspectRatio));
    ui->loginLabel->raise();

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

#if defined(_MSVC_LANG) && (_MSVC_LANG == 202002L) && (DO_SERVER_CONNECT == 1)

     if (!services::SignIn(ui->usernameLineEdit->text().toStdString(), ui->passwordLineEdit->text().toStdString()))
     {
     	QMessageBox msgBox;
         msgBox.setText("No account with username " + ui->usernameLineEdit->text() +
             "and password " + ui->passwordLineEdit->text() +
             " exists. Do you want to create one?");
	
     	QPushButton* yesButton = msgBox.addButton(tr("Yes"), QMessageBox::YesRole);
     	QPushButton* noButton = msgBox.addButton(tr("No"), QMessageBox::NoRole);
     	msgBox.setDefaultButton(noButton);
	
     	msgBox.exec();
	
     	if (msgBox.clickedButton() == yesButton)
     	{
     		if (!services::SignUp(ui->usernameLineEdit->text().toStdString(), ui->passwordLineEdit->text().toStdString()))
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

#endif // MSVC C++20
	
	 /*CanvasPaint child;
	 child.setModal(true);
	 child.exec();*/

	hide();
	child = new CanvasPaint(this);
	child->setDrawState(CanvasPaint::DrawingState::DRAWING);
	child->show();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	QCoreApplication::quit();
	event->accept();
}
