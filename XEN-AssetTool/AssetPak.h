// Author: Jake Rieger
// Created: 5/2/2024.
//

#pragma once

#include <AssetManifest.h>
#include <STL.h>

namespace AssetTool {
    namespace Packer {
        void Pack(const std::vector<AssetManifest*>& manifests);
    }

    namespace UnPacker {
        std::optional<std::unordered_map<std::string, AssetManifest>>
        Unpack(const std::filesystem::path& pakFile, const std::filesystem::path& metaFile);
    }
}  // namespace AssetTool
