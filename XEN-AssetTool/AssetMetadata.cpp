// Author: Jake Rieger
// Created: 5/9/2024.
//

#include "AssetMetadata.h"
#include "AssetTool.h"

#include <cassert>
#include <utility>

namespace AssetTool {
    using namespace Helpers;

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

    ByteArray AssetMetadata::Serialize() {
        ByteArray bytes = {0x0};
        bytes.resize(META_SIZE);

        auto offsetPtrs =
          MemCopy(const_cast<char*>(m_Checksum.c_str()), bytes.data(), m_Checksum.length());
        offsetPtrs = MemCopy(&m_OriginalSize, offsetPtrs.DestPtr, sizeof(size_t));
        offsetPtrs = MemCopy(&m_CompressedSize, offsetPtrs.DestPtr, sizeof(size_t));
        offsetPtrs = MemCopy(&m_ManifestCount, offsetPtrs.DestPtr, sizeof(u32));
        offsetPtrs.Nullify();

        return bytes;
    }

    AssetMetadata AssetMetadata::Deserialize(ByteArray& bytes) {
        char checksum[128];
        size_t originalSize   = 0;
        size_t compressedSize = 0;
        u32 manifestCount     = 0;

        auto offsetPtrs = MemCopy(bytes.data(), &checksum, 128);
        offsetPtrs      = MemCopy(offsetPtrs.SourcePtr, &originalSize, sizeof(size_t));
        offsetPtrs      = MemCopy(offsetPtrs.SourcePtr, &compressedSize, sizeof(size_t));
        offsetPtrs      = MemCopy(offsetPtrs.SourcePtr, &manifestCount, sizeof(u32));
        offsetPtrs.Nullify();

        auto checksumStr = std::string(checksum);
        checksumStr.resize(128);
        return {checksumStr, originalSize, compressedSize, manifestCount};
    }
}  // namespace AssetTool