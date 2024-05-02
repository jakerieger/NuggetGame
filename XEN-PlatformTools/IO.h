// Author: Jake Rieger
// Created: 4/30/2024.
//

#pragma once

#include <STL.h>

namespace PlatformTools::IO {
    namespace FileSystem = std::filesystem;

    std::optional<std::string> Read(const FileSystem::path& filename);
    std::optional<std::vector<u8>> ReadAllBytes(const FileSystem::path& filename);
    std::optional<std::vector<std::string>> ReadAllLines(const FileSystem::path& filename);

    bool Write(const FileSystem::path& filename, const std::string& content);
    bool WriteAllBytes(const FileSystem::path& filename, const std::vector<u8>& bytes);
    bool WriteAllLines(const FileSystem::path& filename, const std::vector<std::string>& lines);
}  // namespace PlatformTools::IO