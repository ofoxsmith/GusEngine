#include "logger.h"

void Logger::Write(LogLevel lvl, std::string msg, std::string source)
{
	std::cout << source << "[" << GetLevelLabel(lvl) << "]: " << msg << endl;
	if (lvl == LogLevel::ERROR || lvl == LogLevel::FATAL) throw new runtime_error(source + "[" + GetLevelLabel(lvl) + "]: " + msg);

}