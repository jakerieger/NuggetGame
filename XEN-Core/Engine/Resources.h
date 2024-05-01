#pragma once

#include "STL.h"

#include <filesystem>

namespace Resources {
    enum class ResourceType : u8 {
        Sprite,
        Sound,
        Font,
        Level,
        ParticleEffect,
    };

    void SetCwd(const char* exePath);
    std::filesystem::path GetRoot();
    std::filesystem::path GetConfigFile();
}  // namespace Resources