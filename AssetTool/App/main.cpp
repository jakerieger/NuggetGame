// Author: Jake Rieger
// Created: 4/30/2024.
//

#include "../Lib/AssetDescriptor.h"

int main() {
    AssetTool::SpriteProperties properties;
    properties.m_Width   = 32;
    properties.m_Height  = 32;
    properties.m_IsAlpha = true;

    const auto bytes = properties.Serialize();
    for (const auto& byte : bytes) {
        printf("0x%02x ", byte);
    }

    AssetTool::SpriteDescriptor spriteDesc;
    spriteDesc.m_Properties = &properties;

    return 0;
}