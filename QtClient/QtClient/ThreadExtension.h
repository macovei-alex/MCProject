#pragma once
#include "macro.h"

#include <QString>

class ThreadExtension
{
public:
	ThreadExtension();
	virtual ~ThreadExtension() = default;

public:
	void Pause();
	void Unpause();
	bool IsPaused() const;

	void Pause(const QString& from);
	void Unpause(const QString& from);

private:
	bool m_pause;
};