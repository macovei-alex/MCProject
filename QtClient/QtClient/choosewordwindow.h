#ifndef CHOOSEWORDWINDOW_H
#define CHOOSEWORDWINDOW_H

#include "macro.h"

#include <QDialog>

class CanvasWindow;

namespace Ui {
class ChooseWordWindow;
}

class ChooseWordWindow : public QDialog
{
	Q_OBJECT

public:
	ChooseWordWindow(QWidget *parent);
	~ChooseWordWindow();
	void setButtonNames(const std::vector<std::string>& words);

private slots:
	void on_firstWordButton_clicked();
	void on_secondWordButton_clicked();
	void on_thirdWordButton_clicked();

signals:
	void Signal();

private:
	Ui::ChooseWordWindow *ui;
	CanvasWindow *canvasWindow;
};

#endif // CHOOSEWORDWINDOW_H
