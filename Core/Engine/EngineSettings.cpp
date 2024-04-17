// Author: Jake Rieger
// Created: 4/9/2024.
//

#include "Resources.h"
#include "EngineSettings.h"
#include "Logger.h"

#include <ini.h>
#include <fstream>
#include <fmt/format.h>

namespace Settings {
    static FEngineSettings g_Settings {
      .Vsync       = true,
      .Fullscreen  = false,
      .ResolutionX = 1280,
      .ResolutionY = 720,
    };

    bool ReadSettings() {
        const mINI::INIFile config(Resources::GetConfigFile().string());
        mINI::INIStructure ini;
        if (!config.read(ini)) {
            Logger::LogError(Logger::Subsystems::RUNTIME,
                             "Unable to read config.ini. File missing or corrupt.");
            return false;
        }

        g_Settings.Vsync       = ini["Settings"]["Vsync"] == "true";
        g_Settings.Fullscreen  = ini["Settings"]["Fullscreen"] == "true";
        g_Settings.ResolutionX = std::stoi(ini["Settings"]["ResolutionX"]);
        g_Settings.ResolutionY = std::stoi(ini["Settings"]["ResolutionY"]);

        Logger::LogInfo(Logger::Subsystems::RUNTIME, "Loaded game settings from config.ini");
        return true;
    }

    void SaveSettings() {
        std::string configOut = "[Settings]\n";

        configOut += fmt::format("Vsync = {}\n", g_Settings.Vsync ? "true" : "false");
        configOut += fmt::format("Fullscreen = {}\n", g_Settings.Fullscreen ? "true" : "false");
        configOut += fmt::format("ResolutionX = {}\n", g_Settings.ResolutionX);
        configOut += fmt::format("ResolutionY = {}\n", g_Settings.ResolutionY);

        if (std::ofstream outStream(Resources::GetConfigFile()); outStream.is_open()) {
            outStream << configOut;
            outStream.close();
            return;
        }
        Logger::LogError(Logger::Subsystems::RUNTIME, "Unable to open file stream for config.ini");
    }

    FEngineSettings& GetSettings() {
        return g_Settings;
    }
}  // namespace Settings