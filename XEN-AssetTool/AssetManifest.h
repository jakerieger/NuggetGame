// Author: Jake Rieger
// Created: 5/1/2024.
//

#pragma once

#include "AssetDescriptor.h"

#include <STL.h>
#include <rapidjson/document.h>

namespace AssetTool {
    class AssetManifest {
    public:
        explicit AssetManifest(const std::filesystem::path& manifest);
        ~AssetManifest();
        std::optional<std::vector<u8>> Serialize();
        std::optional<AssetManifest> Deserialize();
        size_t GetSize();

        u32 m_Version = 0;
        std::string m_Filename;
        std::vector<IAssetDescriptor*> m_Descriptors;

    private:
        void ParseManifest(const rapidjson::Document& manifest);
    };

}  // namespace AssetTool