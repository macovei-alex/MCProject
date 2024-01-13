#include "ThreadExtension.h"

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