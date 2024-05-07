// Author: Jake Rieger
// Created: 5/2/2024.
//

#pragma once

#include "STL.h"

#include <openssl/sha.h>

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

    inline bool ValidateChecksum(const std::vector<u8>& bytesA, const std::vector<u8>& bytesB) {
        u8 hashA[SHA512_DIGEST_LENGTH];
        u8 hashB[SHA512_DIGEST_LENGTH];
        SHA512_CTX ctx;

        SHA512_Init(&ctx);
        SHA512_Update(&ctx, bytesA.data(), bytesA.size());
        SHA512_Final(hashA, &ctx);

        SHA512_Init(&ctx);
        SHA512_Update(&ctx, bytesB.data(), bytesB.size());
        SHA512_Final(hashB, &ctx);

        std::stringstream ssA;
        std::stringstream ssB;
        for (int i = 0; i < SHA512_DIGEST_LENGTH; ++i) {
            ssA << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hashA[i]);
            ssB << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hashB[i]);
        }

        return (ssA.str() == ssB.str());
    }
}  // namespace AssetTool::Helpers