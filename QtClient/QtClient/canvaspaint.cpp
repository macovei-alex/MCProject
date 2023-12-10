#include "canvaspaint.h"
#include "mainwindow.h"
#include "ui_canvaspaint.h"

CanvasPaint::CanvasPaint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CanvasPaint)
{
    ui->setupUi(this);
}

CanvasPaint::~CanvasPaint()
{
    delete ui;
}

void CanvasPaint::on_Button_clicked()
{

}

void CanvasPaint::on_LeaveServerButton_clicked()
{
    hide();
    obiect = new MainWindow(this);
    obiect->show();
}

