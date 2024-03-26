// Author: Jake Rieger
// Created: 3/25/2024.
//

#include "Unpacker.h"

#include <lz4.h>
#include <fstream>
#include <iostream>

void Unpacker::UnpackSprites(const std::filesystem::path& dataDir,
                             std::vector<Packer::Schemas::Sprite>& sprites) {
    // Read the metadata required for decompression
    size_t srcSize           = 0;
    int compressedSize       = 0;
    unsigned int numElements = 0;
    std::vector<size_t> blockSizes;
    {
        // Open NUGMETA file
        std::ifstream metaFile(dataDir / "sprites.nugmeta", std::ios::binary);
        if (!metaFile.is_open()) {
            std::cerr << "(Error) Failed to open sprites.nugmeta" << std::endl;
            return;
        }

        auto metaSize = sizeof(size_t) + sizeof(int) + sizeof(unsigned int);
        auto bytes    = new char[metaSize];
        metaFile.read(bytes, metaSize);

        memcpy_s(&srcSize, sizeof(size_t), bytes, sizeof(size_t));
        memcpy_s(&compressedSize, sizeof(int), bytes + sizeof(size_t), sizeof(int));
        memcpy_s(&numElements,
                 sizeof(unsigned int),
                 bytes + sizeof(size_t) + sizeof(int),
                 sizeof(unsigned int));

        if (srcSize == 0 || compressedSize == 0 || numElements == 0) {
            std::cerr << "(Error) Failed to read one or more metadata values" << std::endl;
            delete[] bytes;
            return;
        }

        delete[] bytes;

        auto blockSizeBytes = new char[numElements * sizeof(size_t)];
        metaFile.read(blockSizeBytes, numElements * sizeof(size_t));
        metaFile.close();

        // Read in block sizes
        for (auto i = 0; i < numElements; i++) {
            auto offset = sizeof(size_t) * i;

            size_t blockSize = 0;
            memcpy_s(&blockSize, sizeof(size_t), blockSizeBytes + offset, sizeof(size_t));

            blockSizes.push_back(blockSize);
        }

        delete[] blockSizeBytes;
    }

    // Decompress the data
    {
        // Open NUGPAK file
        std::ifstream pakFile(dataDir / "sprites.nugpak", std::ios::binary);
        if (!pakFile.is_open()) {
            return;
        }

        const auto bytes             = new char[compressedSize];
        const auto decompressedBytes = new char[srcSize];

        pakFile.read(bytes, compressedSize);
        pakFile.close();

        const int decompressedSize =
          LZ4_decompress_safe(bytes, decompressedBytes, compressedSize, srcSize);
        delete[] bytes;

        if (decompressedSize != srcSize) {
            std::cerr << "(Error) Decompressed data doesn't match original size" << std::endl;
            delete[] decompressedBytes;
            return;
        }

        auto totalBlockSize = 0;
        for (auto i = 0; i < numElements; i++) {
            auto blockSize   = blockSizes.at(i);
            auto spriteBytes = new char[blockSize];
            memcpy_s(spriteBytes, blockSize, decompressedBytes + totalBlockSize, blockSize);

            unsigned int nameLen  = 0;
            unsigned int dataLen  = 0;
            unsigned int width    = 0;
            unsigned int height   = 0;
            unsigned int channels = 0;
            char* name;
            unsigned char* data;

            memcpy_s(&nameLen, sizeof(unsigned int), spriteBytes, sizeof(unsigned int));
            memcpy_s(&dataLen,
                     sizeof(unsigned int),
                     spriteBytes + sizeof(unsigned int),
                     sizeof(unsigned int));
            memcpy_s(&width,
                     sizeof(unsigned int),
                     spriteBytes + sizeof(unsigned int) * 2,
                     sizeof(unsigned int));
            memcpy_s(&height,
                     sizeof(unsigned int),
                     spriteBytes + sizeof(unsigned int) * 3,
                     sizeof(unsigned int));
            memcpy_s(&channels,
                     sizeof(unsigned int),
                     spriteBytes + sizeof(unsigned int) * 4,
                     sizeof(unsigned int));

            name = new char[nameLen];
            strncpy(name, spriteBytes + sizeof(unsigned int) * 5, nameLen);
            name[nameLen - 1] = '\0';

            data = new unsigned char[dataLen];
            memcpy_s(data,
                     dataLen,
                     spriteBytes + sizeof(unsigned int) * 5 + Packer::MAX_STR_LEN,
                     dataLen);

            Packer::Schemas::Sprite sprite(name, width, height, channels, data);
            sprites.push_back(sprite);

            delete[] data;
            delete[] name;
            delete[] spriteBytes;
            totalBlockSize += blockSize;
        }

        delete[] decompressedBytes;
    }
}