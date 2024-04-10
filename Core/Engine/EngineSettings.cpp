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
    static FEngineSettings g_Settings {};

    bool ReadSettings() {
        const auto configPath = Resources::GetRoot() / "config.ini";
        mINI::INIFile config(configPath.string());
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
        const auto configPath = Resources::GetRoot() / "config.ini";
        std::string configOut = "[Settings]\n";

        configOut += fmt::format("Vsync = {}\n", g_Settings.Vsync ? "true" : "false");
        configOut += fmt::format("Fullscreen = {}\n", g_Settings.Fullscreen ? "true" : "false");
        configOut += fmt::format("ResolutionX = {}\n", g_Settings.ResolutionX);
        configOut += fmt::format("ResolutionY = {}\n", g_Settings.ResolutionY);

        if (std::ofstream outStream(configPath); outStream.is_open()) {
            outStream << configOut;
            outStream.close();
        }
        Logger::LogError(Logger::Subsystems::RUNTIME, "Unable to open file stream for config.ini");
    }

    FEngineSettings& GetSettings() {
        return g_Settings;
    }
}  // namespace Settings