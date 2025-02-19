#include "logger.h"

#include <format>
#include <fstream>

std::string_view LogLevelToString(Logger::Level level)
{
	switch (level)
	{
		using enum Logger::Level;
	case Info:
		return "Info";
	case Warning:
		return "Warning";
	case Error:
		return "Error";
	default:
		return "";
	}
}

auto NowInSeconds()
{
	auto nowClock{ std::chrono::system_clock::now() };
	auto nowInSeconds{ std::chrono::duration_cast<std::chrono::seconds>(nowClock.time_since_epoch()) };
	return std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>(nowInSeconds);
}

Logger::Logger(std::ostream& os, Level minimumLevel)
	: m_os{ os }
	, m_minimumLevel{ minimumLevel }
{
	/* EMPTY */
}

Logger::Logger(const std::string& filename, Level minimumLevel)
	: m_minimumLevel{ minimumLevel }
	, temp_outputStream{ filename, std::ios::trunc }
	, m_os{ temp_outputStream }
{
	/* EMPTY */
}

void Logger::SetMinimumLogLevel(Level level)
{
	m_minimumLevel = level;
}

void Logger::Log(const std::string_view& message, Level level)
{
	if ((uint8_t)level < (uint8_t)m_minimumLevel)
		return;

	m_os << std::format("({}) [{}]: {}\n", NowInSeconds(), LogLevelToString(level), message);
}
