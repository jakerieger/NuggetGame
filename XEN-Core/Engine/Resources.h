#pragma once

#include "STL.h"
#include "XPak.h"

#include <filesystem>

namespace Resources {
    void SetCwd(const char* exePath);
    std::filesystem::path GetRoot();
    std::filesystem::path GetConfigFile();

    inline bool UsingPak = true;
    inline std::vector<XPak::FXAsset> Assets;

    inline std::vector<u8> GetAsset(const std::string& path) {
        std::vector<u8> bytes;

        auto assets = Assets;

        if (UsingPak) {
            bytes = XPak::FetchAsset(path, Assets);
        } else {
            bytes = XPak::FetchAssetFromDisk(path);
        }

        return bytes;
    }
}  // namespace Resources