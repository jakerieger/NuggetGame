// Author: Jake Rieger
// Created: 4/7/2024.
//

#pragma once

#include "STL.h"

struct FEngineSettings {
    bool Vsync;
    bool Fullscreen;
    u32 ResolutionX;
    u32 ResolutionY;
};

namespace Settings {
    bool ReadSettings();
    void SaveSettings();
    FEngineSettings& GetSettings();
}  // namespace Settings