#include "ChooseWordWindow.h"
#include "ui_ChooseWordWindow.h"
#include "CanvasWindow.h"

ChooseWordWindow::ChooseWordWindow(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::ChooseWordWindow),
	canvasWindow(nullptr)
{
	ui->setupUi(this);
}

ChooseWordWindow::~ChooseWordWindow()
{
	delete ui;
}

void ChooseWordWindow::setButtonNames(const std::vector<std::string>& words)
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

void ChooseWordWindow::on_firstWordButton_clicked()
{

#ifdef ONLINE
	dynamic_cast<CanvasWindow*>(parentWidget())->SetChosenWord(ui->firstWordButton->text());
	accept();
#endif

}


void ChooseWordWindow::on_secondWordButton_clicked()
{

#ifdef ONLINE
	dynamic_cast<CanvasWindow*>(parentWidget())->SetChosenWord(ui->secondWordButton->text());
	accept();
#endif

}


void ChooseWordWindow::on_thirdWordButton_clicked()
{

#ifdef ONLINE
	dynamic_cast<CanvasWindow*>(parentWidget())->SetChosenWord(ui->thirdWordButton->text());
	accept();
#endif

}

