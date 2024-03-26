// Author: Jake Rieger
// Created: 3/25/2024.
//

#include "Packer.h"
#include "Unpacker.h"
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    std::cout << "Packing game assets...\n\n";

    const std::string exePathStr = argv[0];
#ifdef _WIN32
    const std::string cwdStr = exePathStr.substr(0, exePathStr.find_last_of('\\'));
#else
    const std::string cwdStr = exePathStr.substr(0, exePathStr.find_last_of('/'));
#endif

    const auto assetsRoot = fs::path(cwdStr) / "Assets";
    const auto dataRoot   = fs::path(cwdStr) / "Data";

    if (!exists(assetsRoot)) {
        std::cerr << "(Error) Assets directory is missing!" << std::endl;
        return 1;
    }

    if (!exists(dataRoot)) {
        // Create it
        create_directory(dataRoot);
    }

    const auto error = Packer::PackSprites(assetsRoot / "sprites", dataRoot);
    if (error.has_value()) {
        std::cerr << "(Error) Failed to pack sprites" << std::endl;
        return 2;
    }

    return 0;
}