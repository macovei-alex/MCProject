#ifndef CHOOSEWORDWINDOW_H
#define CHOOSEWORDWINDOW_H

#include <QDialog>

class CanvasPaint;

namespace Ui {
class choosewordwindow;
}

class choosewordwindow : public QDialog
{
    Q_OBJECT

public:
    explicit choosewordwindow(QWidget *parent = nullptr);
    ~choosewordwindow();

private slots:
    void on_firstWordButton_clicked();

    void on_secondWordButton_clicked();

    void on_thirdWordButton_clicked();

private:
    Ui::choosewordwindow *ui;
    CanvasPaint *canvasPaint;
};

#endif // CHOOSEWORDWINDOW_H
