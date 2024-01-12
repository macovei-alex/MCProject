export module logger;

import <iostream>;
import <fstream>;
import <string_view>;
import <string>;
import <chrono>;

export class __declspec(dllexport) Logger
{
public:
	enum class Level : uint8_t
	{
		Info,
		Warning,
		Error
	};

public:
	Logger(std::ostream& os, Level minimumLevel = Level::Info);
	Logger(const std::string& filename, Level minimumLevel = Level::Info);

	void SetMinimumLogLevel(Level level);

	void Log(const std::string_view& message, Level level = Level::Info);

private:
	std::ostream& m_os;
	Level m_minimumLevel;
	std::ofstream temp_outputStream;
};
