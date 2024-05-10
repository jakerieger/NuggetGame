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
    using namespace Helpers;

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
        // Size, Name Length, Descriptor Count
        const size_t baseSize = SizeOfAll<size_t, u32, u32>();
        // Number of bytes in Name
        const size_t nameLen = m_Name.length();
        // Combined size of all descriptors
        const int descriptorSize =
          std::accumulate(m_Descriptors.begin(),
                          m_Descriptors.end(),
                          0,
                          [](int acc, const std::unique_ptr<IAssetDescriptor>& descriptor) {
                              return acc + static_cast<int>(descriptor->GetSize());
                          });
        return static_cast<size_t>(descriptorSize) + nameLen + baseSize;
    }
}  // namespace AssetTool