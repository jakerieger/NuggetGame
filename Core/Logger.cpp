// Author: Jake Rieger
// Created: 3/31/2024.
//

#include "Logger.h"
#include <cstdarg>

#define NOW (unsigned long)time(NULL)
#define RESET_COLOR "\033[0m"
#define BLACK_COLOR "\033[30m"
#define RED_COLOR "\033[31m"
#define GREEN_COLOR "\033[32m"
#define YELLOW_COLOR "\033[33m"
#define BLUE_COLOR "\033[34m"
#define MAGENTA_COLOR "\033[35m"
#define CYAN_COLOR "\033[36m"
#define WHITE_COLOR "\033[37m"
#define BOLD_BLACK_COLOR "\033[1m\033[30m"
#define BOLD_RED_COLOR "\033[1m\033[31m"
#define BOLD_GREEN_COLOR "\033[1m\033[32m"
#define BOLD_YELLOW_COLOR "\033[1m\033[33m"
#define BOLD_BLUE_COLOR "\033[1m\033[34m"
#define BOLD_MAGENTA_COLOR "\033[1m\033[35m"
#define BOLD_CYAN_COLOR "\033[1m\033[36m"
#define BOLD_WHITE_COLOR "\033[1m\033[37m"
#define BG_BLACK_COLOR "\033[40m"
#define BG_RED_COLOR "\033[41m"
#define BG_GREEN_COLOR "\033[42m"
#define BG_YELLOW_COLOR "\033[43m"
#define BG_BLUE_COLOR "\033[44m"
#define BG_MAGENTA_COLOR "\033[45m"
#define BG_CYAN_COLOR "\033[46m"
#define BG_WHITE_COLOR "\033[47m"

namespace Logger {
    std::vector<FLogEntry> g_LogEntries;
    std::unordered_map<ELogLevel, const char*> g_LevelMap = {
      {ELogLevel::Info, "INFO"},
      {ELogLevel::Warning, "WARNING"},
      {ELogLevel::Error, "ERROR"},
      {ELogLevel::Fatal, "FATAL"},
    };

    static std::string GetTimestamp() {
        time_t rawtime;
        char buffer[80];
        time(&rawtime);
        const tm* timeinfo = localtime(&rawtime);
        strftime(buffer, 80, "%I:%M %p", timeinfo);
        auto timestamp = std::string(buffer);

        return timestamp;
    }

    static void PrintColoredMessage(const char* colorCode,
                                    const char* logLevel,
                                    const char* subsystem,
                                    const char* message) {
        printf("%s|%s| [%s] (%s) %s%s\n",
               colorCode,
               GetTimestamp().c_str(),
               logLevel,
               subsystem,
               message,
               RESET_COLOR);
    }

    static void Log(const FLogEntry& entry) {
        g_LogEntries.push_back(entry);

        const char* logLevel  = g_LevelMap.find(entry.Level)->second;
        const char* subsystem = entry.Subsystem.c_str();
        const char* message   = entry.Message.c_str();

        switch (entry.Level) {
            case ELogLevel::Info: {
                PrintColoredMessage(WHITE_COLOR, logLevel, subsystem, message);
            } break;
            case ELogLevel::Warning: {
                PrintColoredMessage(YELLOW_COLOR, logLevel, subsystem, message);
            } break;
            case ELogLevel::Error: {
                PrintColoredMessage(RED_COLOR, logLevel, subsystem, message);
            } break;
            case ELogLevel::Fatal: {
                PrintColoredMessage(BOLD_RED_COLOR, logLevel, subsystem, message);
            } break;
        }
    }

    static void FormatMessage(std::string& out, const char* format, va_list args) {
        const size_t bufferSize = vsnprintf(nullptr, 0, format, args);
        const auto buffer       = new char[bufferSize];
        vsprintf(buffer, format, args);
        out = STRDUP(buffer);
    }

    static void CreateLogEntry(const ELogLevel level,
                               const std::string& subsystem,
                               const char* format,
                               va_list args) {
        FLogEntry entry = {};
        entry.Subsystem = subsystem;
        entry.Timestamp = NOW;
        entry.Level     = level;
        FormatMessage(entry.Message, format, args);
        Log(entry);
    }

    void LogInfo(const std::string& subsystem, const char* format, ...) {
        va_list args;
        va_start(args, format);
        CreateLogEntry(ELogLevel::Info, subsystem, format, args);
        va_end(args);
    }

    void LogWarning(const std::string& subsystem, const char* format, ...) {
        va_list args;
        va_start(args, format);
        CreateLogEntry(ELogLevel::Warning, subsystem, format, args);
        va_end(args);
    }

    void LogError(const std::string& subsystem, const char* format, ...) {
        va_list args;
        va_start(args, format);
        CreateLogEntry(ELogLevel::Error, subsystem, format, args);
        va_end(args);
    }

    void LogFatal(const std::string& subsystem, const char* format, ...) {
        va_list args;
        va_start(args, format);
        CreateLogEntry(ELogLevel::Fatal, subsystem, format, args);
        va_end(args);
    }

    void Dump(std::vector<FLogEntry>& out) {}

    void DumpToDisk(const std::filesystem::path& filename) {}
}  // namespace Logger