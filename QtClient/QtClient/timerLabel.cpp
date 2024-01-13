#include "TimerLabel.h"
#include <QTimer>

TimerLabel::TimerLabel(QWidget* parent) : QLabel(parent), timer(new QTimer(this)), remainingTime(0)
{
    connect(timer, &QTimer::timeout, this, &TimerLabel::updateTimer);
}
void TimerLabel::stopTimer()
{
    timer->stop();
}
void TimerLabel::startTimer()
{
    remainingTime = 60;
    updateTimer();
    timer->start(1000);
}
void TimerLabel::updateTimer()
{
    setText(QString::number(remainingTime) + "s");
	remainingTime--;
	if(remainingTime < 0)
	{
        stopTimer();
	}
}

