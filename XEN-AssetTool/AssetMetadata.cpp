// Author: Jake Rieger
// Created: 5/9/2024.
//

#include "AssetMetadata.h"
#include "AssetTool.h"

#include <cassert>
#include <utility>

namespace AssetTool {
    // 128-byte checksum + 8-byte size + 8-byte size + 4-byte uint
    static constexpr size_t META_SIZE = 148;

    AssetMetadata::AssetMetadata(std::string checksum,
                                 const size_t originalSize,
                                 const size_t compressedSize,
                                 const u32 manifestCount)
        : m_Checksum(std::move(checksum)), m_OriginalSize(originalSize),
          m_CompressedSize(compressedSize), m_ManifestCount(manifestCount) {
        assert(m_Checksum.length() == 128);
    }

    ByteArray AssetMetadata::Serialize() const {
        ByteArray bytes = {0x0};
        bytes.resize(META_SIZE);

        auto insertPoint =
          Helpers::MemCopyDest(m_Checksum.c_str(), bytes.data(), m_Checksum.length());
        insertPoint = Helpers::MemCopyDest(&m_OriginalSize, insertPoint, sizeof(size_t));
        insertPoint = Helpers::MemCopyDest(&m_CompressedSize, insertPoint, sizeof(size_t));
        insertPoint = Helpers::MemCopyDest(&m_ManifestCount, insertPoint, sizeof(u32));

        return bytes;
    }

    AssetMetadata AssetMetadata::Deserialize(ByteArray& bytes) {
        char checksum[128];
        size_t originalSize   = 0;
        size_t compressedSize = 0;
        u32 manifestCount     = 0;

        auto offset = Helpers::MemCopySrc(bytes.data(), &checksum, 128);
        offset      = Helpers::MemCopySrc(offset, &originalSize, sizeof(size_t));
        offset      = Helpers::MemCopySrc(offset, &compressedSize, sizeof(size_t));
        offset      = Helpers::MemCopySrc(offset, &manifestCount, sizeof(u32));
        offset      = nullptr;

        auto checksumStr = std::string(checksum);
        checksumStr.resize(128);
        return {checksumStr, originalSize, compressedSize, manifestCount};
    }
}  // namespace AssetTool