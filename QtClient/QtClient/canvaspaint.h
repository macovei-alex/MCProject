#ifndef CANVASPAINT_H
#define CANVASPAINT_H
#include <QDialog>
#include <QPainter>
#include <QMouseEvent>
#include<qpixmap.h>

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
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void resizeEvent(QResizeEvent* event) override;
    void clearCanvas();
    QPixmap canvasPixmap;
protected:
    void paintEvent(QPaintEvent* event) override; // Asigură-te că override este specificat aici
private slots:
    void on_Button_clicked();
    void on_LeaveServerButton_clicked();
    void on_ResetCanvas_clicked();
   // void minimizeButtonClicked();

    void on_DrawButton_clicked();

    void on_EraseButton_clicked();

private:
    bool isDrawing;
    bool isErasing;
    Ui::CanvasPaint *ui;
     QPoint lastPoint;
    MainWindow *obiect;
 
};

#endif // CANVASPAINT_H
