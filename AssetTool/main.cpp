// Author: Jake Rieger
// Created: 4/30/2024.
//

#include "AssetDescriptor.h"

int main() {
    AssetTool::SpriteDescriptor spriteDesc;
    spriteDesc.m_Name    = "Nugget";
    spriteDesc.m_Type    = AssetTool::AssetType::Sprite;
    spriteDesc.m_Version = 1;
    spriteDesc.m_SrcData = {0x69, 0x69, 0x69, 0x69, 0x69, 0x69, 0x69};

    const auto properties = spriteDesc.GetProperties();
    properties->m_Width   = 32;
    properties->m_Height  = 32;
    properties->m_IsAlpha = true;

    const auto bytes = spriteDesc.Serialize();
    for (const auto& byte : bytes) {
        printf("0x%02x ", byte);
    }

    return 0;
}