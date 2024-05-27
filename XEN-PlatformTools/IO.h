// Author: Jake Rieger
// Created: 4/30/2024.
//

#pragma once

#include <filesystem>
#include <optional>
#include <vector>
#include <cstdint>

namespace IO {
    namespace FileSystem = std::filesystem;

    std::optional<std::string> Read(const FileSystem::path& filename);
    std::optional<std::vector<uint8_t>> ReadAllBytes(const FileSystem::path& filename);
    std::optional<std::vector<std::string>> ReadAllLines(const FileSystem::path& filename);
    std::optional<std::vector<uint8_t>>
    ReadBlock(const FileSystem::path& filename, uint32_t blockOffset, size_t blockSize);

    bool Write(const FileSystem::path& filename, const std::string& content);
    bool WriteAllBytes(const FileSystem::path& filename, const std::vector<uint8_t>& bytes);
    bool WriteAllLines(const FileSystem::path& filename, const std::vector<std::string>& lines);
}  // namespace IO