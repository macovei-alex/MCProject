#include "TimerLabel.h"
#include <QTimer>

TimerLabel::TimerLabel(QWidget* parent) : QLabel(parent), timer(new QTimer(this)), remainingTime(0)
{
    connect(timer, &QTimer::timeout, this, &TimerLabel::updateTimer);
}
void TimerLabel::stopTimer()
{
    //EMPTY
}
void TimerLabel::startTimer()
{
    //EMPTY
}
void TimerLabel::updateTimer()
{
	//EMPTY
}

