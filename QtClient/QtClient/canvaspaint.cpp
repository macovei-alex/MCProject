#include "canvaspaint.h"
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
