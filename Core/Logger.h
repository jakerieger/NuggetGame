// Author: Jake Rieger
// Created: 3/31/2024.
//

#pragma once

#include "STL.h"

#include <filesystem>

namespace Logger {
    namespace Subsystems {
        static const std::string GRAPHICS = "Core::Graphics";
        static const std::string RUNTIME  = "Core::Runtime";
        static const std::string PHYSICS  = "Core::Physics";
        static const std::string AUDIO    = "Core::Audio";
        static const std::string INPUT    = "Core::Input";
        static const std::string UI       = "Core::UI";
    }  // namespace Subsystems

    enum class ELogLevel : u8 {
        Info,
        Warning,
        Error,
        Fatal,
    };

    struct FLogEntry {
        ELogLevel Level;
        std::string Subsystem;
        std::string Message;
        time_t Timestamp;
    };

    void LogInfo(const std::string& subsystem, const char* _restrict format, ...);
    void LogWarning(const std::string& subsystem, const char* _restrict format, ...);
    void LogError(const std::string& subsystem, const char* _restrict format, ...);
    void LogFatal(const std::string& subsystem, const char* _restrict format, ...);
    void Dump(std::vector<FLogEntry>& out);
    void DumpToDisk(const std::filesystem::path& filename);
}  // namespace Logger