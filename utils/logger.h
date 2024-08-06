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
    void Debug(string msg) { Write(DEBUG, msg); }
    void Debug(string source, string msg) { Write(DEBUG, msg, source); }
    void Info(string msg) { Write(INFO, msg); }
    void Info(string source, string msg) { Write(INFO, msg, source); }
    void Warn(string msg) { Write(WARN, msg); }
    void Warn(string source, string msg) { Write(WARN, msg, source); }
    void Error(string msg) { Write(ERROR, msg); }
    void Error(string source, string msg) { Write(ERROR, msg, source); }
    void FatalError(string msg) { Write(FATAL, msg); }
    void FatalError(string source, string msg) { Write(FATAL, msg, source); }
};