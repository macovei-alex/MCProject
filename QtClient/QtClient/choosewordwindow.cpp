#include "choosewordwindow.h"
#include "ui_choosewordwindow.h"

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
