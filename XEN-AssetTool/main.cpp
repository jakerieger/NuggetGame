// Author: Jake Rieger
// Created: 4/30/2024.
//

#include "AssetDescriptor.h"
#include "AssetManifest.h"

using namespace AssetTool;

int main() {
    const AssetManifest spriteManifest("Sprites.manifest");

    size_t totalBytes = 0;
    for (const auto& descriptor : spriteManifest.m_Descriptors) {
        const auto size = descriptor->GetSize();
        totalBytes += size;
        printf("Size: %llu bytes\n", size);
    }
    printf("\nTotal Size: %llu bytes\n\n", totalBytes);

    spriteManifest.Serialize();

    return 0;
}