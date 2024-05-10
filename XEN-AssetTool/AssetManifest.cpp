// Author: Jake Rieger
// Created: 5/1/2024.
//

#include "AssetManifest.h"
#include "PlatformTools.h"

#include <numeric>
#include <fmt/core.h>

namespace AssetTool {
    using namespace PlatformTools;
    using namespace rapidjson;

    static bool ValidateManifest(const Document& manifest) {
        if (!manifest.IsObject())
            return false;
        if (!manifest.HasMember("version") || !manifest["version"].IsNumber())
            return false;
        if (!manifest.HasMember("assets") || !manifest["assets"].IsArray())
            return false;

        return true;
    }

    void AssetManifest::ParseManifest(const Document& manifest) {}

    AssetManifest::AssetManifest(const std::filesystem::path& manifest) {}

    AssetManifest::~AssetManifest() {}

    std::optional<std::vector<u8>> AssetManifest::Serialize() {
        return {};
    }

    std::unique_ptr<AssetManifest> AssetManifest::Deserialize(const std::vector<u8>& bytes) {
        return nullptr;
    }

    size_t AssetManifest::GetSize() {
        return 0;
    }
}  // namespace AssetTool