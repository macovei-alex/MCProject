#ifndef CHOOSEWORDWINDOW_H
#define CHOOSEWORDWINDOW_H

#include <QDialog>

namespace Ui {
class choosewordwindow;
}

class choosewordwindow : public QDialog
{
    Q_OBJECT

public:
    explicit choosewordwindow(QWidget *parent = nullptr);
    ~choosewordwindow();

private:
    Ui::choosewordwindow *ui;
};

#endif // CHOOSEWORDWINDOW_H
