// Author: Jake Rieger
// Created: 3/31/2024.
//

#include "Logger.h"
#include <cstdarg>

#define NOW (unsigned long)time(NULL)

namespace Logger {
    std::vector<FLogEntry> g_LogEntries;
    std::unordered_map<ELogLevel, const char*> g_LevelMap = {
      {ELogLevel::Info, "INFO"},
      {ELogLevel::Warning, "WARNING"},
      {ELogLevel::Error, "ERROR"},
      {ELogLevel::Fatal, "FATAL"},
    };

    /// https://stackoverflow.com/a/69004694/5521933
    /// TODO: Figure out why this don't work :(
    static void FormatTime(std::string& out, const time_t timestamp) {
        tm ts             = {};
        ts                = *localtime(&timestamp);
        const auto buffer = new char[80];
        strftime(buffer, sizeof(buffer), "%H:%M:%S %Z", &ts);
        out = _strdup(buffer);
    }

    static void Log(const FLogEntry& entry) {
        g_LogEntries.push_back(entry);

        // Format timestamp into HH::MM::SS AM/PM
        std::string timestamp;
        FormatTime(timestamp, entry.Timestamp);

        if (entry.Level >= ELogLevel::Error) {
            fprintf(stderr,
                    "[%s] (%s) %s\n",
                    g_LevelMap.find(entry.Level)->second,
                    entry.Subsystem.c_str(),
                    entry.Message.c_str());
        } else {
            fprintf(stdout,
                    "[%s] (%s) %s\n",
                    g_LevelMap.find(entry.Level)->second,
                    entry.Subsystem.c_str(),
                    entry.Message.c_str());
        }
    }

    static void FormatMessage(std::string& out, const char* format, va_list args) {
        const size_t bufferSize = snprintf(nullptr, 0, format, args);
        const auto buffer       = new char[bufferSize];
        sprintf(buffer, format, args);
        out = _strdup(buffer);
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