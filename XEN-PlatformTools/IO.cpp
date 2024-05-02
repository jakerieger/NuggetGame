// Author: Jake Rieger
// Created: 4/30/2024.
//

#include "IO.h"

#include <fstream>

namespace PlatformTools::IO {
    std::optional<std::string> Read(const FileSystem::path& filename) {
        if (!exists(filename) || is_directory(filename)) {
            return std::nullopt;
        }

        std::ifstream file(filename);
        if (!file.is_open()) {
            return std::nullopt;
        }

        std::string content((std::istreambuf_iterator(file)), std::istreambuf_iterator<char>());
        file.close();

        return content;
    }

    std::optional<std::vector<u8>> ReadAllBytes(const FileSystem::path& filename) {
        if (!exists(filename) || is_directory(filename)) {
            return std::nullopt;
        }

        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            return std::nullopt;
        }

        std::vector<u8> bytes(std::istreambuf_iterator(file), {});
        file.close();

        return bytes;
    }

    std::optional<std::vector<std::string>> ReadAllLines(const FileSystem::path& filename) {
        if (!exists(filename) || is_directory(filename)) {
            return std::nullopt;
        }

        std::ifstream file(filename);
        if (!file.is_open()) {
            return std::nullopt;
        }

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }

        file.close();

        return lines;
    }

    bool Write(const FileSystem::path& filename, const std::string& content) {
        std::ofstream outfile(filename);
        if (!outfile.is_open()) {
            return false;
        }

        outfile.write(content.c_str(), (std::streamsize)content.length());
        outfile.close();

        return true;
    }

    bool WriteAllBytes(const FileSystem::path& filename, const std::vector<u8>& bytes) {
        std::ofstream outfile(filename, std::ios::binary);
        if (!outfile.is_open()) {
            return false;
        }

        outfile.write(reinterpret_cast<const char*>(bytes.data()), (std::streamsize)bytes.size());
        outfile.close();

        return true;
    }

    bool WriteAllLines(const FileSystem::path& filename, const std::vector<std::string>& lines) {
        std::ofstream outfile(filename);
        if (!outfile.is_open()) {
            return false;
        }

        for (const auto& line : lines) {
            outfile << line << std::endl;
        }

        outfile.close();

        return true;
    }
}  // namespace PlatformTools::IO