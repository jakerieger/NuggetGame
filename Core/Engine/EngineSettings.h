// Author: Jake Rieger
// Created: 4/7/2024.
//

#pragma once

#include "STL.h"

struct FEngineSettings {
    struct Graphics {
        bool Vsync;
        bool Fullscreen;
        u32 ResolutionX;
        u32 ResolutionY;
    } Graphics;
    struct Audio {
        f32 VolumeMaster;
        f32 VolumeUI;
        f32 VolumeFX;
        f32 VolumeMusic;
    } Audio;
};

namespace Settings {
    bool ReadSettings();
    void SaveSettings();
    FEngineSettings& GetSettings();
}  // namespace Settings