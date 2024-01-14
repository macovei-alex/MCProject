#include "choosewordwindow.h"
#include "ui_choosewordwindow.h"
#include "canvaspaint.h"

choosewordwindow::choosewordwindow(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::choosewordwindow),
	canvasPaint(nullptr)
{
	ui->setupUi(this);
}

choosewordwindow::~choosewordwindow()
{
	delete ui;
}

void choosewordwindow::setButtonNames(const std::vector<std::string>& words)
{
	if (words.size() == 3)
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

void choosewordwindow::on_firstWordButton_clicked()
{
	dynamic_cast<CanvasPaint*>(parentWidget())->GetOnlineData().chosenWord = ui->firstWordButton->text();
	accept();
}


void choosewordwindow::on_secondWordButton_clicked()
{
	dynamic_cast<CanvasPaint*>(parentWidget())->GetOnlineData().chosenWord = ui->secondWordButton->text();
	accept();
}


void choosewordwindow::on_thirdWordButton_clicked()
{
	dynamic_cast<CanvasPaint*>(parentWidget())->GetOnlineData().chosenWord = ui->thirdWordButton->text();
	accept();
}

