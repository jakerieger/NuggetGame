#pragma once

#include "Packer.h"
#include "STL.h"
#include <filesystem>

#define RES_SPRITE "sprites"
#define RES_ROOT ""

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
    std::string GetResource(const char* type, const char* name);
    std::vector<Packer::Schemas::Sprite>& GetSprites();

    template<typename T>
    T& GetResource(const ResourceType type, const char* name) {
        if (type == ResourceType::Sprite) {
            static_assert(std::is_same_v<T, Packer::Schemas::Sprite>, "T is not Sprite type");

            for (auto& sprite : GetSprites()) {
                if (strcmp(sprite.name, name) == 0) {
                    return sprite;
                }
            }
        }
    }
}  // namespace Resources