// Author: Jake Rieger
// Created: 5/2/2024.
//

#pragma once

namespace AssetTool::Helpers {
    template<typename... Types>
    size_t SizeOfAll() {
        return (sizeof(Types) + ...);
    }

    /// @brief This is basically just a re-implementation of `memcpy` that returns the next
    /// point in memory to insert
    ///
    /// @return Next offset in source memory to copy to
    inline const char*
    CopyMemory(void* source, void* destination, const size_t size, const size_t offset = 0) {
        const auto src = static_cast<char*>(source);
        const auto dst = static_cast<char*>(destination);

        for (size_t i = 0; i < size; i++) {
            dst[i] = src[i + offset];
        }

        return src + size + offset;
    }
}  // namespace AssetTool::Helpers
