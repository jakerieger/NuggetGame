// Author: Jake Rieger
// Created: 5/2/2024.
//

#include "AssetPak.h"

#include "IO.h"

#include <lz4.h>
#include <numeric>

namespace AssetTool {
    struct FPakMetadata {
        size_t OriginalSize;
        size_t CompressedSize;
    };

    void Packer::Pack(const std::vector<AssetManifest*>& manifests) {
        std::vector<u8> pakBytes;
        const int reserveSize = std::accumulate(
          manifests.begin(),
          manifests.end(),
          0,
          [](int acc, AssetManifest* manifest) { return acc + (int)manifest->GetSize(); });
        pakBytes.resize(reserveSize);

        FPakMetadata pakMetadata = {};
        pakMetadata.OriginalSize = reserveSize;

        size_t offset = 0;
        for (auto& manifest : manifests) {
            auto result = manifest->Serialize();
            if (!result.has_value()) {
                printf("Unable to serialize manifest '%s'\n", manifest->m_Filename.c_str());
                break;
            }
            auto bytes = result.value();
            memcpy(pakBytes.data() + offset, bytes.data(), bytes.size());

            offset += manifest->GetSize();
        }

        const u8* sourceBytes   = pakBytes.data();
        const size_t sourceSize = pakBytes.size();
        const size_t maxDstSize = LZ4_compressBound((int)sourceSize);

        const auto compressedBytes = new char[maxDstSize];
        const size_t compressedSize =
          LZ4_compress_default(reinterpret_cast<const char*>(sourceBytes),
                               compressedBytes,
                               (int)sourceSize,
                               (int)maxDstSize);

        std::vector<u8> compressedBytesVec;
        compressedBytesVec.resize(compressedSize);
        memcpy(compressedBytesVec.data(), reinterpret_cast<u8*>(compressedBytes), compressedSize);
        delete compressedBytes;

        pakMetadata.CompressedSize = compressedSize;

        PlatformTools::IO::WriteAllBytes("data0.nugpak", compressedBytesVec);
    }

    std::optional<AssetManifest> UnPacker::Unpack(const std::filesystem::path& pakFile,
                                                  const std::filesystem::path& metaFile) {
        return std::nullopt;
    }
}  // namespace AssetTool