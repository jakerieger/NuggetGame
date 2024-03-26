// Author: Jake Rieger
// Created: 3/25/2024.
//

#include "Packer.h"

#include "STL.h"
#include "stb_image.h"

#include <cassert>
#include <cstring>
#include <fstream>

#ifdef _WIN32
    #include <algorithm>
#else
    #include <bits/ranges_algo.h>
#endif

namespace Packer {
    namespace fs = std::filesystem;

    namespace Schemas {
        Sprite::Sprite(const char* _name,
                       const unsigned int _width,
                       const unsigned int _height,
                       const unsigned int _channels,
                       const unsigned char* _data)
            : width(_width), height(_height), channels(_channels) {
            nameLen = strlen(_name) + 1;
            name    = new char[MAX_STR_LEN];
            strncpy(name, _name, nameLen);
            name[nameLen] = '\0';
            data          = new unsigned char[width * height * 4];  // Assumes RGBA format
            memcpy(data, _data, width * height * 4);
            dataLen = width * height * 4;
        }
    }  // namespace Schemas

    std::optional<PackError> PackSprites(const std::filesystem::path& spritesDir,
                                         const std::filesystem::path& dataDir) {
        std::vector<Schemas::Sprite> sprites;

        // For each sprite asset in our Assets directory
        {
            std::ranges::for_each(
              fs::directory_iterator {spritesDir},
              [&](const fs::directory_entry& dirEntry) {
                  const auto entry = dirEntry.path().string();
                  int width, height, nrComponents;
                  stbi_set_flip_vertically_on_load(true);
                  unsigned char* data = stbi_load(entry.c_str(), &width, &height, &nrComponents, 0);
                  if (data) {
                      const auto filename = dirEntry.path().filename().string();
                      sprites.emplace_back(filename.c_str(), width, height, nrComponents, data);
                  }
                  stbi_image_free(data);
              });
        }

        // Compress raw bytes and write metadata
        std::vector<char> rawBytes;
        char* compressedData;
        int compressedDataSize;
        {
            for (const auto& sprite : sprites) {
                // Copy width, height, and name
                rawBytes.insert(rawBytes.end(),
                                reinterpret_cast<const char*>(&sprite.nameLen),
                                reinterpret_cast<const char*>(&sprite.nameLen) +
                                  sizeof(sprite.nameLen));
                rawBytes.insert(rawBytes.end(),
                                reinterpret_cast<const char*>(&sprite.dataLen),
                                reinterpret_cast<const char*>(&sprite.dataLen) +
                                  sizeof(sprite.dataLen));
                rawBytes.insert(rawBytes.end(),
                                reinterpret_cast<const char*>(&sprite.width),
                                reinterpret_cast<const char*>(&sprite.width) +
                                  sizeof(sprite.width));
                rawBytes.insert(rawBytes.end(),
                                reinterpret_cast<const char*>(&sprite.height),
                                reinterpret_cast<const char*>(&sprite.height) +
                                  sizeof(sprite.height));
                rawBytes.insert(rawBytes.end(),
                                reinterpret_cast<const char*>(&sprite.channels),
                                reinterpret_cast<const char*>(&sprite.channels) +
                                  sizeof(sprite.channels));
                rawBytes.insert(rawBytes.end(),
                                reinterpret_cast<const char*>(sprite.name),
                                reinterpret_cast<const char*>(sprite.name) +
                                  MAX_STR_LEN);  // Include null terminator

                // Copy image data
                rawBytes.insert(rawBytes.end(),
                                sprite.data,
                                sprite.data +
                                  sprite.width * sprite.height * 4);  // Assuming RGBA format
            }

            const size_t srcSize = sizeof(char) * rawBytes.size();

            // Write original for debugging
            // {
            //     std::ofstream origFile(dataDir / "sprites_orig.nugpak", std::ios::binary);
            //     origFile.write(rawBytes.data(), srcSize);
            //     origFile.close();
            // }

            const size_t maxDstSize = LZ4_compressBound(static_cast<int>(srcSize));
            compressedData          = static_cast<char*>(malloc(maxDstSize));
            compressedDataSize      = LZ4_compress_default(rawBytes.data(),
                                                      compressedData,
                                                      static_cast<int>(srcSize),
                                                      static_cast<int>(maxDstSize));
            if (compressedDataSize <= 0) {
                return PackError::ERROR_COMPRESSION_FAILED;
            }

            // ReSharper disable once CppDFAConstantConditions
            if (compressedDataSize > 0) {
                printf("Done. Ratio: %.2f\n",
                       static_cast<float>(compressedDataSize) / static_cast<float>(srcSize));
            }

            compressedData =
              static_cast<char*>(realloc(compressedData, static_cast<size_t>(compressedDataSize)));
            if (!compressedData) {
                return PackError::ERROR_REALLOC_FAILED;
            }

            // Write NUGMETA file
            auto outFile = dataDir / "sprites.nugmeta";
            std::ofstream metaFile(outFile, std::ios::binary);
            if (!metaFile.is_open()) {
                return PackError::ERROR_OPEN_FILE_FOR_WRITE_FAILED;
            }

            // NUGMETA binary layout:
            // (size_t)                  (int)         (uint)        size_t[]
            // 00 00 00 00 00 00 00 00 | 00 00 00 00 | 00 00 00 00 | [00 00 00 00 00 00 00 00]...
            // Origin size               Comp size     Num elements  Block sizes
            auto numElements = static_cast<unsigned int>(sprites.size());
            metaFile.write(reinterpret_cast<const char*>(&srcSize), sizeof(size_t));
            metaFile.write(reinterpret_cast<const char*>(&compressedDataSize), sizeof(int));
            metaFile.write(reinterpret_cast<const char*>(&numElements), sizeof(unsigned int));

            std::vector<char> blockSizeBytes;
            for (auto i = 0; i < sprites.size(); i++) {
                auto sprite      = sprites.at(i);
                size_t blockSize = 0;
                blockSize += sizeof(unsigned int) * 5;
                blockSize += sprite.dataLen;
                blockSize += MAX_STR_LEN;

                std::cout << blockSize << std::endl;

                blockSizeBytes.insert(blockSizeBytes.end(),
                                      reinterpret_cast<const char*>(&blockSize),
                                      reinterpret_cast<const char*>(&blockSize) + sizeof(size_t));
            }

            metaFile.write(blockSizeBytes.data(), sizeof(size_t) * sprites.size());
            metaFile.close();
        }

        // Write compressed data to file
        {
            auto outFile = dataDir / "sprites.nugpak";
            std::ofstream pakFile(outFile, std::ios::binary);
            if (!pakFile.is_open())
                return PackError::ERROR_OPEN_FILE_FOR_WRITE_FAILED;

            pakFile.write(compressedData, compressedDataSize);
            pakFile.close();
        }

        // Cleanup
        {
            for (const auto& sprite : sprites) {
                delete[] sprite.data;
                delete[] sprite.name;
            }
        }

        return {};
    }
}  // namespace Packer