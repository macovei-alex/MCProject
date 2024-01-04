#include "mainwindow.h"
#include "canvaspaint.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QPixmap>
//#include <QQuickWidget>

#include "services.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	qDebug() << "MainWindow constructor";

	ui->setupUi(this);
	QPixmap pix(":Resource Files/Images/Background.jpg");
	int width = ui->label->width();
	int height = ui->label->height();
	ui->label->setPixmap(pix.scaled(width, height, Qt::KeepAspectRatio));
	QPixmap pix1(":Resource Files/Images/login_icon.png");
	ui->label_login->setPixmap(pix1.scaled(100, 100, Qt::KeepAspectRatio));
	ui->label_login->raise();
	/* QQuickWidget *qmlWidget = new QQuickWidget();
	 qmlWidget->setSource(QUrl("qrc:/Canvas.qml"));
	 ui->centralwidget->layout()->addWidget(qmlWidget);
	 QObject:: connect(qmlWidget->rootObject(), SIGNAL(qmlSignal()), this, SLOT(handelQmlSignal()));*/
}


MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_LoginButton_clicked()
{
	if (ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty())
	{
		QMessageBox::warning(this, "Login", "Username or password is empty");
		return;
	}

	if (!services::SignIn(ui->lineEdit->text().toStdString(), ui->lineEdit_2->text().toStdString()))
	{
		QMessageBox::warning(this, "Login", "Username or password is incorrect");
		return;
	}

	// CanvasPaint obiect;
	// obiect.setModal(true);
	// obiect.exec();

	hide();
	child = new CanvasPaint(this);
	child->setDrawingFlag(true);
	child->show();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	QCoreApplication::quit();
	event->accept();
}
