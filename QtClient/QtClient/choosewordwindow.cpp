#include "choosewordwindow.h"
#include "ui_choosewordwindow.h"
#include "canvaspaint.h"

choosewordwindow::choosewordwindow(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::choosewordwindow)
    
{
    ui->setupUi(this);
    LoadWords();
}

choosewordwindow::~choosewordwindow()
{
    delete ui;
}

void choosewordwindow::HandleCloseChoosingWindow()
{
    hide();
}

QStringList choosewordwindow::readWordsFromFile(const QString& filePath)
{
    return QStringList();
}

QStringList choosewordwindow::loadRandomWords(int count)
{
    return QStringList();
}

void choosewordwindow::LoadWords()
{
   
}

void choosewordwindow::reloadWords()
{
}
void  choosewordwindow::openCanvasPaintWithWord(const QString& chosenWord)
{
    accept();
    if (!canvasPaint)
    {
        canvasPaint = new CanvasPaint(this);
    }
    //canvasPaint->setChosenWord(chosenWord);
    canvasPaint->show();
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

