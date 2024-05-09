// Author: Jake Rieger
// Created: 5/2/2024.
//

#pragma once

#include <AssetManifest.h>
#include <STL.h>

namespace AssetTool {
    namespace Packer {
        void Pack(const std::vector<std::unique_ptr<AssetManifest>>& manifests);
    }

    namespace UnPacker {
        auto Unpack(const std::filesystem::path& pakFile, const std::filesystem::path& metaFile)
          -> std::optional<std::unordered_map<std::string, std::unique_ptr<AssetManifest>>>;
    }
}  // namespace AssetTool
