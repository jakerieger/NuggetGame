// Author: Jake Rieger
// Created: 5/2/2024.
//

#include "AssetPak.h"
#include "IO.h"

#include <lzma.h>
#include <numeric>

namespace AssetTool {
    struct FPakMetadata {
        size_t OriginalSize;
        size_t CompressedSize;
        u32 ManifestCount;

        [[nodiscard]] std::vector<u8> Serialize() const {}

        void Deserialize(const std::vector<u8>& bytes) {}
    };

    void Packer::Pack(const std::vector<std::unique_ptr<AssetManifest>>& manifests) {}

    static std::optional<std::vector<u8>> DecompressPakFile(const std::vector<u8>& data,
                                                            const FPakMetadata& metadata) {}

    auto UnPacker::Unpack(const std::filesystem::path& pakFile,
                          const std::filesystem::path& metaFile)
      -> std::optional<std::unordered_map<std::string, std::unique_ptr<AssetManifest>>> {}
}  // namespace AssetTool