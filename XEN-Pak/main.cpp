// Author: Jake Rieger
// Created: 5/21/2024.
//

#include "IO.h"
#include "XPak.h"

#include <filesystem>
#include <CLI/CLI.hpp>
#include <yaml-cpp/yaml.h>

struct FPackOptions {
    std::string Manifest = "manifest.yaml";
    std::string OutDir   = "Data";
};

struct FExtractOptions {
    std::string PakFile;
    std::string OutDir;
};

struct FPrintOptions {
    std::string PakFile;
};

struct FImportOptions {
    std::string AssetPath;
    std::string AssetType;
};

int main(int argc, char* argv[]) {
    CLI::App app {"XPak CLI Tool"};
    argv = app.ensure_utf8(argv);

    const auto cmdPack = app.add_subcommand("pack", "Pack assets");
    FPackOptions packOptions;

    cmdPack->add_option("-i,--input", packOptions.Manifest, "Manifest file")
      ->check(CLI::ExistingFile);
    cmdPack->add_option("-o,--outdir", packOptions.OutDir, "Directory to write pak files to");

    const auto cmdExtract = app.add_subcommand("extract", "Extract asset from pak file");
    FExtractOptions extractOptions;

    cmdExtract->add_option("-i,--input", extractOptions.PakFile, "Pak file")
      ->required()
      ->check(CLI::ExistingFile);
    cmdExtract->add_option("-o,--outdir",
                           extractOptions.OutDir,
                           "Directory to extract pak files to");

    const auto cmdPrint = app.add_subcommand("print", "Print contents and info from pak file");
    FPrintOptions printOptions;

    cmdPrint->add_option("-i,--input", printOptions.PakFile, "Pak file")
      ->required()
      ->check(CLI::ExistingFile);

    CLI11_PARSE(app, argc, argv);

    if (argc == 1 || app.get_subcommands().empty()) {
        std::cout << app.help() << std::endl;
        return 0;
    }

    if (cmdPack->parsed()) {
        const auto manifest = YAML::LoadFile(packOptions.Manifest);
        XPak::Pack(manifest, packOptions.OutDir);
    }

    if (cmdExtract->parsed()) {
        if (!std::filesystem::exists(extractOptions.OutDir)) {
            std::filesystem::create_directory(extractOptions.OutDir);
        }

        for (const auto assets = XPak::Unpack(extractOptions.PakFile);
             const auto& [Entry, PakFile, Properties] : assets) {
            auto blockResult = IO::ReadBlock(PakFile, Entry.BlockOffset, Entry.BlockSize);
            if (!blockResult.has_value()) {
                std::cerr << "Unable to extract asset from pak file\n";
                break;
            }
            auto outBytes              = XPak::Decompress(blockResult.value());
            std::filesystem::path path = Entry.Path;
            auto filename              = path.filename();
            IO::WriteAllBytes(extractOptions.OutDir / filename, outBytes);
        }
    }

    if (cmdPrint->parsed()) {
    }

    return 0;
}