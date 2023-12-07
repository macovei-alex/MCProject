#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPixmap>
//#include <QQuickWidget>
#include "Canvas.qml"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
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
    

}
void MainWindow::handelQmlSignal()
{
	QMessageBox::information(this, "QML", "QML Signal Received");
}


