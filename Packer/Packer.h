// Author: Jake Rieger
// Created: 3/25/2024.
//

#pragma once

#include "Types.h"

#include <iostream>
#include <optional>
#include <filesystem>
#include <vector>

namespace Packer {
    static constexpr int MAX_STR_LEN = 64;

    namespace Schemas {
        struct Sprite {
            unsigned int nameLen;
            unsigned int dataLen;
            unsigned int width;
            unsigned int height;
            unsigned int channels;
            char* name;
            unsigned char* data;

            Sprite(const char* _name,
                   unsigned int _width,
                   unsigned int _height,
                   unsigned int _channels,
                   const unsigned char* _data);
        };

        struct Level {
            u32 mapRows;
            u32 mapColumns;
            struct Vec2D {
                float x;
                float y;
            } playerStart;
            Vec2D objectivePosition;
            size_t tileCount;
            std::vector<i32> tileData;

            Level(u32 rows, u32 cols, Vec2D start, Vec2D obj, const std::vector<i32>& tileData);
        };
    }  // namespace Schemas

    enum class PackError : unsigned char {
        ERROR_IMAGE_LOADING_FAILED,
        ERROR_OPEN_FILE_FOR_WRITE_FAILED,
        ERROR_COMPRESSION_FAILED,
        ERROR_REALLOC_FAILED
    };

    std::optional<PackError> PackSprites(const std::filesystem::path& spritesDir,
                                         const std::filesystem::path& dataDir);
}  // namespace Packer