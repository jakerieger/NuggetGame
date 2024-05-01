// Author: Jake Rieger
// Created: 4/30/2024.
//

#pragma once

#include <STL.h>

namespace PlatformTools::IO {
    namespace fs = std::filesystem;

    std::optional<std::string> Read(const fs::path& filename);
    std::optional<std::vector<u8>> ReadAllBytes(const fs::path& filename);
    std::optional<std::vector<std::string>> ReadAllLines(const fs::path& filename);

    bool Write(const fs::path& filename, const std::string& content);
    bool WriteAllBytes(const fs::path& filename, const std::vector<u8>& bytes);
    bool WriteAllLines(const fs::path& filename, const std::vector<std::string>& lines);
}  // namespace PlatformTools::IO