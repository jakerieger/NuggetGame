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
      .Graphics {
        .Vsync       = true,
        .Fullscreen  = false,
        .ResolutionX = 1600,
        .ResolutionY = 900,
      },
      .Audio {
        .VolumeMaster = 1.f,
        .VolumeUI     = 0.8f,
        .VolumeFX     = 1.f,
        .VolumeMusic  = 0.4f,
      },
    };

    bool ReadSettings() {
        const mINI::INIFile config(Resources::GetConfigFile().string());
        mINI::INIStructure ini;
        if (!config.read(ini)) {
            Logger::LogError(Logger::Subsystems::RUNTIME,
                             "Unable to read config.ini. File missing or corrupt.");
            return false;
        }

        g_Settings.Graphics.Vsync       = ini["Graphics"]["Vsync"] == "true";
        g_Settings.Graphics.Fullscreen  = ini["Graphics"]["Fullscreen"] == "true";
        g_Settings.Graphics.ResolutionX = std::stoi(ini["Graphics"]["ResolutionX"]);
        g_Settings.Graphics.ResolutionY = std::stoi(ini["Graphics"]["ResolutionY"]);

        g_Settings.Audio.VolumeMaster = std::stof(ini["Audio"]["VolumeMaster"]);
        g_Settings.Audio.VolumeUI     = std::stof(ini["Audio"]["VolumeUI"]);
        g_Settings.Audio.VolumeFX     = std::stof(ini["Audio"]["VolumeFX"]);
        g_Settings.Audio.VolumeMusic  = std::stof(ini["Audio"]["VolumeMusic"]);

        Logger::LogInfo(Logger::Subsystems::RUNTIME, "Loaded game settings from config.ini");
        return true;
    }

    void SaveSettings() {
        std::string configOut = "[Graphics]\n";

        configOut += fmt::format("Vsync = {}\n", g_Settings.Graphics.Vsync ? "true" : "false");
        configOut +=
          fmt::format("Fullscreen = {}\n", g_Settings.Graphics.Fullscreen ? "true" : "false");
        configOut += fmt::format("ResolutionX = {}\n", g_Settings.Graphics.ResolutionX);
        configOut += fmt::format("ResolutionY = {}\n", g_Settings.Graphics.ResolutionY);

        configOut += "\n[Audio]\n";
        configOut += fmt::format("VolumeMaster = {}\n", g_Settings.Audio.VolumeMaster);
        configOut += fmt::format("VolumeUI = {}\n", g_Settings.Audio.VolumeUI);
        configOut += fmt::format("VolumeFX = {}\n", g_Settings.Audio.VolumeFX);
        configOut += fmt::format("VolumeMusic = {}\n", g_Settings.Audio.VolumeMusic);

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