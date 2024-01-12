#include "choosewordwindow.h"
#include "ui_choosewordwindow.h"
#include "canvaspaint.h"

choosewordwindow::choosewordwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::choosewordwindow)
{
    ui->setupUi(this);
}

choosewordwindow::~choosewordwindow()
{
    delete ui;
}

void choosewordwindow::on_firstWordButton_clicked()
{
    reject();

    if(!canvasPaint)
    {
        canvasPaint = new CanvasPaint(this);
    }
    canvasPaint->show();
}


void choosewordwindow::on_secondWordButton_clicked()
{
    reject();

    if(!canvasPaint)
    {
        canvasPaint = new CanvasPaint(this);
    }
    canvasPaint->show();
}


void choosewordwindow::on_thirdWordButton_clicked()
{
    reject();

    if(!canvasPaint)
    {
        canvasPaint = new CanvasPaint(this);
    }
    canvasPaint->show();
}

