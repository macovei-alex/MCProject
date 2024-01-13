#ifndef TIMERLABEL_H
#define TIMERLABEL_H

#include <QLabel>
#include <QTimer>
class TimerLabel : public QLabel
{
	Q_OBJECT
public:
    TimerLabel(QWidget* parent = nullptr);

public slots:
    void startTimer();
    void stopTimer();

private slots:
    void updateTimer();

private:
    QTimer* timer;
    uint64_t remainingTime;
};
#endif 


