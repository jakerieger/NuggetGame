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

        printf("\nCreating asset pak. This might take a minute...\n");

        const u8* sourceBytes   = pakBytes.data();
        const size_t sourceSize = pakBytes.size();

        lzma_stream strm = LZMA_STREAM_INIT;
        lzma_ret ret     = lzma_easy_encoder(&strm, LZMA_PRESET_DEFAULT, LZMA_CHECK_CRC64);

        if (ret != LZMA_OK) {
            throw std::runtime_error("Error initializing the LZMA encoder.");
        }

        strm.avail_in = sourceSize;
        strm.next_in  = sourceBytes;

        std::vector<u8> compressed;
        compressed.resize(sourceSize);

        strm.avail_out = sourceSize;
        strm.next_out  = compressed.data();

        ret = lzma_code(&strm, LZMA_FINISH);

        if (ret != LZMA_OK && ret != LZMA_STREAM_END) {
            lzma_end(&strm);
            throw std::runtime_error("Error compressing data.");
        }

        compressed.resize(compressed.size() - strm.avail_out);
        lzma_end(&strm);
        PlatformTools::IO::WriteAllBytes("data0.nugpak", compressed);

        printf("\nDone. Created pak file => '%s'\n", "data0.nugpak");
        printf("Compression ratio: %.02f%%\n",
               ((float)compressed.size() / (float)sourceSize) * 100.f);
        printf("Compression size: %d bytes => %d bytes\n", (int)sourceSize, (int)compressed.size());
    }

    std::optional<AssetManifest> UnPacker::Unpack(const std::filesystem::path& pakFile,
                                                  const std::filesystem::path& metaFile) {
        return std::nullopt;
    }
}  // namespace AssetTool