// Author: Jake Rieger
// Created: 4/30/2024.
//

#include "AssetDescriptor.h"
#include "PlatformTools.h"
#include "AssetTool.h"

#include <iostream>
#include <numeric>

namespace AssetTool {
    using namespace Helpers;

    IProperties::~IProperties()           = default;
    IAssetDescriptor::~IAssetDescriptor() = default;

    void SpriteProperties::Deserialize(std::vector<u8>& bytes) {}
    void FontProperties::Deserialize(std::vector<u8>& bytes) {}
    void AudioProperties::Deserialize(std::vector<u8>& bytes) {}
    void LevelProperties::Deserialize(std::vector<u8>& bytes) {}

    std::vector<u8> SpriteProperties::Serialize() {
        return {};
    }
    std::vector<u8> FontProperties::Serialize() {
        return {};
    }
    std::vector<u8> AudioProperties::Serialize() {
        return {};
    }
    std::vector<u8> LevelProperties::Serialize() {
        return {};
    }

    SpriteProperties* SpriteDescriptor::GetProperties() const {
        return dynamic_cast<SpriteProperties*>(m_Properties);
    }
    FontProperties* FontDescriptor::GetProperties() const {
        return dynamic_cast<FontProperties*>(m_Properties);
    }
    AudioProperties* AudioDescriptor::GetProperties() const {
        return dynamic_cast<AudioProperties*>(m_Properties);
    }
    LevelProperties* LevelDescriptor::GetProperties() const {
        return dynamic_cast<LevelProperties*>(m_Properties);
    }

    SpriteDescriptor::SpriteDescriptor() {
        this->m_Properties = new SpriteProperties;
        this->m_Type       = AssetType::Sprite;
    }
    FontDescriptor::FontDescriptor() {
        this->m_Properties = new FontProperties;
        this->m_Type       = AssetType::Font;
    }
    AudioDescriptor::AudioDescriptor() {
        this->m_Properties = new AudioProperties;
        this->m_Type       = AssetType::Audio;
    }
    LevelDescriptor::LevelDescriptor() {
        this->m_Properties = new LevelProperties;
        this->m_Type       = AssetType::Level;
    }

    std::vector<u8> IAssetDescriptor::Serialize() {
        return {};
    }

}  // namespace AssetTool