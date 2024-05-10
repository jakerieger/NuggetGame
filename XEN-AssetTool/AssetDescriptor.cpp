// Author: Jake Rieger
// Created: 4/30/2024.
//

#include "AssetDescriptor.h"
#include "AssetTool.h"

#include <iostream>
#include <numeric>

namespace AssetTool {
    using namespace Helpers;

    IProperties::~IProperties()           = default;
    IAssetDescriptor::~IAssetDescriptor() = default;

    void SpriteProperties::Deserialize(ByteArray& bytes) {
        auto offset = MemCopySrc(bytes.data(), &m_Width, sizeof(u32));
        offset      = MemCopySrc(offset, &m_Height, sizeof(u32));
        offset      = MemCopySrc(offset, &m_IsAlpha, sizeof(bool));
    }
    void FontProperties::Deserialize(ByteArray& bytes) {
        auto offset = MemCopySrc(bytes.data(), &m_DefaultSize, sizeof(u32));
    }
    void AudioProperties::Deserialize(ByteArray& bytes) {
        auto offset = MemCopySrc(bytes.data(), &m_SampleCount, sizeof(u32));
        offset      = MemCopySrc(offset, &m_SampleRate, sizeof(u32));
        offset      = MemCopySrc(offset, &m_Channels, sizeof(u32));
    }
    void LevelProperties::Deserialize(ByteArray& bytes) {
        auto offset = MemCopySrc(bytes.data(), &m_Rows, sizeof(u32));
        offset      = MemCopySrc(offset, &m_Columns, sizeof(u32));
        offset      = MemCopySrc(offset, &m_PlayerStart.x, sizeof(f32));
        offset      = MemCopySrc(offset, &m_PlayerStart.y, sizeof(f32));
        offset      = MemCopySrc(offset, &m_ObjectivePosition.x, sizeof(f32));
        offset      = MemCopySrc(offset, &m_ObjectivePosition.y, sizeof(f32));
    }

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

    ByteArray IAssetDescriptor::Serialize() const {
        ByteArray bytes   = {0x0};
        const size_t size = GetSize();
        bytes.resize(size);

        const size_t nameLen       = m_Name.length();
        const size_t propsLen      = m_Properties->GetSize();
        const size_t dataLen       = m_SrcData.size();
        const ByteArray propsBytes = m_Properties->Serialize();

        auto insertPtr = MemCopyDest(&size, bytes.data(), sizeof(size_t));
        insertPtr      = MemCopyDest(&m_Type, insertPtr, sizeof(u8));
        insertPtr      = MemCopyDest(&m_Version, insertPtr, sizeof(u32));
        insertPtr      = MemCopyDest(&nameLen, insertPtr, sizeof(u32));
        insertPtr      = MemCopyDest(m_Name.c_str(), insertPtr, nameLen);
        insertPtr      = MemCopyDest(&propsLen, insertPtr, sizeof(u32));
        insertPtr      = MemCopyDest(propsBytes.data(), insertPtr, propsLen);
        insertPtr      = MemCopyDest(&dataLen, insertPtr, sizeof(u32));
        insertPtr      = MemCopyDest(m_SrcData.data(), insertPtr, dataLen);
        insertPtr      = nullptr;

        return bytes;
    }

    std::unique_ptr<IAssetDescriptor> AssetDescriptor::Deserialize(ByteArray& data) {
        size_t size               = 0;
        u8 type                   = 0;
        u32 version               = 0;
        u32 nameLen               = 0;
        u32 propsLen              = 0;
        ByteArray propertiesBytes = {0x0};
        u32 dataLen               = 0;
        ByteArray dataBytes       = {0x0};

        auto offsetPtr = MemCopySrc(data.data(), &size, sizeof(size_t));
        offsetPtr      = MemCopySrc(offsetPtr, &type, sizeof(u8));
        offsetPtr      = MemCopySrc(offsetPtr, &version, sizeof(u32));
        offsetPtr      = MemCopySrc(offsetPtr, &nameLen, sizeof(u32));

        const auto name = new char[nameLen];
        offsetPtr       = MemCopySrc(offsetPtr, name, nameLen);
        name[nameLen]   = '\0';

        offsetPtr = MemCopySrc(offsetPtr, &propsLen, sizeof(u32));
        propertiesBytes.resize(propsLen);

        offsetPtr = MemCopySrc(offsetPtr, propertiesBytes.data(), propsLen);
        offsetPtr = MemCopySrc(offsetPtr, &dataLen, sizeof(u32));
        dataBytes.resize(dataLen);

        offsetPtr = MemCopySrc(offsetPtr, dataBytes.data(), dataLen);

        const std::string nameStr = STRDUP(name);
        std::unique_ptr<IAssetDescriptor> descriptor;

        const auto assetType = static_cast<AssetType>(type);
        switch (assetType) {
            case AssetType::Sprite: {
                descriptor = std::make_unique<SpriteDescriptor>();
            } break;
            case AssetType::Font: {
                descriptor = std::make_unique<FontDescriptor>();
            } break;
            case AssetType::Audio: {
                descriptor = std::make_unique<AudioDescriptor>();
            } break;
            case AssetType::Level: {
                descriptor = std::make_unique<LevelDescriptor>();
            } break;
            default:
                return nullptr;
        }

        descriptor->m_Name    = nameStr;
        descriptor->m_Version = version;
        descriptor->m_Type    = assetType;
        descriptor->m_SrcData = dataBytes;

        return descriptor;
    }

}  // namespace AssetTool