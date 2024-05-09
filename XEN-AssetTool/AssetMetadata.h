// Author: Jake Rieger
// Created: 5/9/2024.
//

#pragma once

#include "STL.h"

namespace AssetTool {
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

        [[nodiscard]] ByteArray Serialize() const;
        static AssetMetadata Deserialize(ByteArray& bytes);
    };
}  // namespace AssetTool
