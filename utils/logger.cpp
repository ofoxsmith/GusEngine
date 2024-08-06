#include "logger.h"

void Logger::Write(LogLevel lvl, std::string msg, std::string source)
{
	if (lvl == FATAL) throw new runtime_error(source + "[FATAL]: " + msg);

	std::cout << source << "[" << GetLevelLabel(lvl) << "]: " << msg << endl;
}