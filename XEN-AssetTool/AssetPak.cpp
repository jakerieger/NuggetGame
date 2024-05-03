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

        auto result = PlatformTools::IO::WriteAllBytes("data0.adf", pakBytes);

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

        pakMetadata.CompressedSize = compressed.size();
        std::vector<u8> metaBytes;
        metaBytes.resize(sizeof(size_t) * 2);
        memcpy(metaBytes.data(), &pakMetadata.OriginalSize, sizeof(size_t));
        memcpy(metaBytes.data() + sizeof(size_t), &pakMetadata.CompressedSize, sizeof(size_t));
        PlatformTools::IO::WriteAllBytes("data0.nugmeta", metaBytes);

        printf("\nDone. \nPak file => '%s'\nMeta file => '%s'\n", "data0.nugpak", "data0.nugmeta");
        printf("Compression ratio: %.02f%%\n",
               ((float)compressed.size() / (float)sourceSize) * 100.f);
        printf("Compression size: %d bytes => %d bytes\n", (int)sourceSize, (int)compressed.size());
    }

    static std::optional<std::vector<u8>> DecompressPakFile(const std::vector<u8>& data,
                                                            const size_t originalSize,
                                                            const size_t compressedSize) {
        std::vector<u8> uncompressedBytes;
        uncompressedBytes.reserve(originalSize);

        assert(compressedSize == data.size());

        lzma_stream strm = LZMA_STREAM_INIT;
        lzma_ret ret     = lzma_stream_decoder(&strm, UINT64_MAX, 0);
        if (ret != LZMA_OK) {
            return std::nullopt;
        }

        strm.avail_in = data.size();
        strm.next_in  = data.data();

        do {
            u8 buffer[BUFSIZ];
            strm.avail_out = BUFSIZ;
            strm.next_out  = buffer;
            ret            = lzma_code(&strm, LZMA_RUN);
            if (ret != LZMA_OK && ret != LZMA_STREAM_END) {
                lzma_end(&strm);
                return std::nullopt;
            }
            uncompressedBytes.insert(uncompressedBytes.end(),
                                     buffer,
                                     buffer + BUFSIZ - strm.avail_out);
            uncompressedBytes.resize(originalSize);
        } while (ret != LZMA_STREAM_END);

        lzma_end(&strm);

        return uncompressedBytes;
    }

    std::optional<AssetManifest> UnPacker::Unpack(const std::filesystem::path& pakFile,
                                                  const std::filesystem::path& metaFile) {
        const auto readPakResult = PlatformTools::IO::ReadAllBytes(pakFile);
        if (!readPakResult.has_value()) {
            return std::nullopt;
        }
        const auto readMetaResult = PlatformTools::IO::ReadAllBytes(metaFile);
        if (!readMetaResult.has_value()) {
            return std::nullopt;
        }

        const auto& pakBytes  = readPakResult.value();
        const auto& metaBytes = readMetaResult.value();

        size_t originalSize;
        size_t compressedSize;
        memcpy(&originalSize, metaBytes.data(), sizeof(size_t));
        memcpy(&compressedSize, metaBytes.data() + sizeof(size_t), sizeof(size_t));

        auto decompressedBytes = DecompressPakFile(pakBytes, originalSize, compressedSize);

        return std::nullopt;
    }
}  // namespace AssetTool