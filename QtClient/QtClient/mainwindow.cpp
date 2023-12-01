#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix("C:/Users/usER/Desktop/Anul_II/Semestrul_I/MC/Proiect/Background.jpg");
    int width = ui->label->width();
    int height = ui->label->height();
    ui->label->setPixmap(pix.scaled(width, height, Qt::KeepAspectRatio));
}

MainWindow::~MainWindow()
{
    delete ui;
}

