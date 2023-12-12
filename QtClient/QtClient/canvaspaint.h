#ifndef CANVASPAINT_H
#define CANVASPAINT_H
#include <QDialog>

class MainWindow;

namespace Ui {
class CanvasPaint;
}

class CanvasPaint : public QDialog
{
    Q_OBJECT

public:
    CanvasPaint(QWidget *parent = nullptr);
    ~CanvasPaint();

private slots:
    void on_Button_clicked();
    void on_LeaveServerButton_clicked();

private:
    Ui::CanvasPaint *ui;
    MainWindow *obiect;
};

#endif // CANVASPAINT_H
