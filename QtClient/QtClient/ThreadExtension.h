#pragma once

class ThreadExtension
{
public:
	ThreadExtension();
	virtual ~ThreadExtension() = default;

public:
	void Pause();
	void Unpause();
	bool IsPaused() const;

private:
	bool m_pause;
};