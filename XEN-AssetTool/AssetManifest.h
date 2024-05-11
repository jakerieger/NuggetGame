// Author: Jake Rieger
// Created: 5/1/2024.
//

#pragma once

#include "AssetDescriptor.h"

#include <STL.h>
#include <rapidjson/document.h>

#include <utility>

namespace AssetTool {
    class AssetManifest {
    public:
        AssetManifest(const u32 version,
                      std::string filename,
                      std::vector<std::unique_ptr<IAssetDescriptor>> descriptors)
            : m_Version(version), m_Name(std::move(filename)),
              m_Descriptors(std::move(descriptors)) {}
        explicit AssetManifest(const std::filesystem::path& manifest);
        ~AssetManifest();
        ByteArray Serialize();
        static std::unique_ptr<AssetManifest> Deserialize(const std::vector<u8>& bytes);
        size_t GetSize();

        u32 m_Version = 0;
        std::string m_Name;
        std::vector<std::unique_ptr<IAssetDescriptor>> m_Descriptors;

    private:
        void ParseManifest(const rapidjson::Document& manifest);
    };

}  // namespace AssetTool