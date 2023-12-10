#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

class CanvasPaint;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_LoginButton_clicked();
   

private:
    Ui::MainWindow *ui;
    CanvasPaint *obiect;
};
#endif // MAINWINDOW_H
