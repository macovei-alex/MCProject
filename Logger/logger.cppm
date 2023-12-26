export module logger;

import <iostream>;
import <string_view>;
import <string>;


export class __declspec(dllexport) Logger
{
public:
	enum class Level : char
	{
		Info,
		Warning,
		Error
	};

public:
	Logger(std::ostream& os, Level minimumLevel = Level::Info);

	void SetMinimumLogLevel(Level level);

	void Log(const std::string_view& messeage, Level level);

private:
	std::ostream& m_os;
	Level m_minimumLevel;
};
