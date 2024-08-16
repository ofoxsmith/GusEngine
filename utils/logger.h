#pragma once

#include <iostream>

using namespace std;
enum LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

class Logger {
    private:
    void Write(LogLevel lvl, string msg, string source = "");
    inline string GetLevelLabel(LogLevel lvl) {
        switch (lvl) {
            case DEBUG: return "DEBUG";
            case INFO: return "INFO";
            case WARN: return "WARN";
            case ERROR: return "ERROR";
            case FATAL: return "FATAL";
        }
        return "UNKNOWN";
    }
    public:
    inline void Debug(string msg) { Write(DEBUG, msg); }
    inline void Debug(string source, string msg) { Write(DEBUG, msg, source); }
    inline void Info(string msg) { Write(INFO, msg); }
    inline void Info(string source, string msg) { Write(INFO, msg, source); }
    inline void Warn(string msg) { Write(WARN, msg); }
    inline void Warn(string source, string msg) { Write(WARN, msg, source); }
    inline void Error(string msg) { Write(ERROR, msg); }
    inline void Error(string source, string msg) { Write(ERROR, msg, source); }
    inline void FatalError(string msg) { Write(FATAL, msg); }
    inline void FatalError(string source, string msg) { Write(FATAL, msg, source); }
};