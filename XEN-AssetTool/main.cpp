// Author: Jake Rieger
// Created: 4/30/2024.
//

#include "AssetManifest.h"
#include "AssetPak.h"

#include <PlatformTools.h>

using namespace AssetTool;
using namespace PlatformTools;
using namespace rapidjson;

int Pack() {
    IO::FileSystem::path manifestsPath = "manifests.json";

    if (!IO::FileSystem::exists("manifests.json")) {
        printf("Could not find manifests.json file.\n");
        return -1;
    }

    const auto manifests = IO::Read(manifestsPath);
    if (!manifests.has_value()) {
        printf("Could not read manifests.json.\n");
        return -1;
    }
    const auto& manifestsSrc = manifests.value();
    Document manifestsJson;
    manifestsJson.Parse(manifestsSrc.c_str());
    if (!manifestsJson.IsObject() || !manifestsJson.HasMember("manifests") ||
        !manifestsJson["manifests"].IsArray()) {
        printf("Invalid manifests.json.\n");
        return -1;
    }

    printf("Processing provided manifest files...\n\n");

    std::vector<AssetManifest*> manifestsToPack;
    const auto manifestFiles = manifestsJson["manifests"].GetArray();
    for (int i = 0; i < manifestFiles.Size(); i++) {
        auto _manifest = new AssetManifest(manifestFiles[i].GetString());
        manifestsToPack.push_back(_manifest);
    }

    Packer::Pack(manifestsToPack);

    for (const auto _manifest : manifestsToPack) {
        delete _manifest;
    }

    return 0;
}

int Unpack() {
    const IO::FileSystem::path pakFile  = "data0.nugpak";
    const IO::FileSystem::path metaFile = "data0.nugmeta";

    const auto result = UnPacker::Unpack(pakFile, metaFile);
    assert(result.has_value());

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("[AssetTool]: Incorrect usage.\n\nArguments:\n  --pack\n  --unpack\n");
        return -1;
    }

    if (argc == 2) {
        if (strcmp(argv[1], "--pack") == 0) {
            return Pack();
        }
        if (strcmp(argv[1], "--unpack") == 0) {
            return Unpack();
        }
        printf("[AssetTool]: Unknown argument '%s'.\n\nArguments:\n  --pack\n  --unpack\n",
               argv[1]);
    }

    return -1;
}