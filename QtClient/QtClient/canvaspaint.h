#ifndef CANVASPAINT_H
#define CANVASPAINT_H

#include <QDialog>

namespace Ui {
class CanvasPaint;
}

class CanvasPaint : public QDialog
{
    Q_OBJECT

public:
    explicit CanvasPaint(QWidget *parent = nullptr);
    ~CanvasPaint();

private:
    Ui::CanvasPaint *ui;
};

#endif // CANVASPAINT_H
