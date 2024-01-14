#include "choosewordwindow.h"
#include "ui_choosewordwindow.h"
#include "canvaspaint.h"

choosewordwindow::choosewordwindow(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::choosewordwindow),
    canvasPaint(nullptr)
    
{
    ui->setupUi(this);
    if (auto canvas = dynamic_cast<CanvasPaint*>(parent))
    {
        canvas->setChooseWordWindowOpen(true);
        canvasPaint = canvas;
    }
   
}

choosewordwindow::~choosewordwindow()
{

    if (canvasPaint)
    {
        canvasPaint->setChooseWordWindowOpen(false);
    }
    delete ui;
}

void choosewordwindow::setButtonNames(const std::vector<std::string>& words)
{
    if (words.size() >= 3)
    {
        ui->firstWordButton->setText(QString::fromStdString(words[0]));
        ui->secondWordButton->setText(QString::fromStdString(words[1]));
        ui->thirdWordButton->setText(QString::fromStdString(words[2]));
    }
    else
    {
        
        qDebug() << "Error: Insufficient words .";
    }
}

QString choosewordwindow::getChosenWord() const
{
    return chosenWord;
}

void choosewordwindow::on_firstWordButton_clicked()
{
    QString chosenWord = ui->firstWordButton->text();
    
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

