// Author: Jake Rieger
// Created: 5/9/2024.
//

#pragma once

#include "STL.h"

namespace AssetTool {
    // 128-byte checksum + 8-byte size + 8-byte size + 4-byte uint
    static constexpr size_t META_SIZE = 148;

    class AssetMetadata {
    public:
        AssetMetadata() = default;
        AssetMetadata(std::string checksum,
                      size_t originalSize,
                      size_t compressedSize,
                      u32 manifestCount);

        std::string m_Checksum;
        size_t m_OriginalSize   = 0;
        size_t m_CompressedSize = 0;
        u32 m_ManifestCount     = 0;

        [[nodiscard]] ByteArray Serialize();
        static AssetMetadata Deserialize(ByteArray& bytes);
    };
} // namespace AssetTool
