// Author: Jake Rieger
// Created: 4/30/2024.
//

#include "AssetManifest.h"
#include "AssetPak.h"

#include <PlatformTools.h>

using namespace AssetTool;
using namespace PlatformTools;
using namespace rapidjson;

int main() {
    if (!IO::FileSystem::exists("manifests.json")) {
        printf("Could not find manifests.json file.\n");
        return -1;
    }

    const auto manifests = IO::Read("manifests.json");
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