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

    void SpriteProperties::Deserialize(ByteArray& bytes) {}
    void FontProperties::Deserialize(ByteArray& bytes) {}
    void AudioProperties::Deserialize(ByteArray& bytes) {}
    void LevelProperties::Deserialize(ByteArray& bytes) {}

    ByteArray SpriteProperties::Serialize() {
        ByteArray bytes = {0x0};
        bytes.resize(GetSize());

        auto insertPtr = MemCopyDest(&m_Width, bytes.data(), sizeof(u32));
        insertPtr      = MemCopyDest(&m_Height, insertPtr, sizeof(u32));
        insertPtr      = MemCopyDest(&m_IsAlpha, insertPtr, sizeof(bool));

        return bytes;
    }
    ByteArray FontProperties::Serialize() {
        ByteArray bytes = {0x0};
        bytes.resize(GetSize());

        auto insertPtr = MemCopyDest(&m_DefaultSize, bytes.data(), sizeof(u32));

        return bytes;
    }
    ByteArray AudioProperties::Serialize() {
        ByteArray bytes = {0x0};
        bytes.resize(GetSize());

        auto insertPtr = MemCopyDest(&m_SampleCount, bytes.data(), sizeof(u32));
        insertPtr      = MemCopyDest(&m_SampleRate, insertPtr, sizeof(u32));
        insertPtr      = MemCopyDest(&m_Channels, insertPtr, sizeof(u32));

        return bytes;
    }
    ByteArray LevelProperties::Serialize() {
        ByteArray bytes = {0x0};
        bytes.resize(GetSize());

        auto insertPtr = MemCopyDest(&m_Rows, bytes.data(), sizeof(u32));
        insertPtr      = MemCopyDest(&m_Columns, insertPtr, sizeof(u32));
        insertPtr      = MemCopyDest(&m_PlayerStart.x, insertPtr, sizeof(f32));
        insertPtr      = MemCopyDest(&m_PlayerStart.y, insertPtr, sizeof(f32));
        insertPtr      = MemCopyDest(&m_ObjectivePosition.x, insertPtr, sizeof(f32));
        insertPtr      = MemCopyDest(&m_ObjectivePosition.y, insertPtr, sizeof(f32));

        return bytes;
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

    ByteArray IAssetDescriptor::Serialize() {
        return {};
    }

}  // namespace AssetTool