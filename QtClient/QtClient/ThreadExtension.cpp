#include "ThreadExtension.h"

#ifdef ONLINE
#include <QDebug>

ThreadExtension::ThreadExtension() :
	m_pause{ false }
{
	/* empty */
}

void ThreadExtension::Pause()
{
	m_pause = true;
}

void ThreadExtension::Unpause()
{
	m_pause = false;
}

bool ThreadExtension::IsPaused() const
{
	return m_pause;
}

void ThreadExtension::Pause(const QString& from)
{
	qDebug() << from << " paused";
	m_pause = true;
}

void ThreadExtension::Unpause(const QString& from)
{
	qDebug() << from << " unpaused";
	m_pause = false;
}
#endif