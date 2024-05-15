// Author: Jake Rieger
// Created: 4/30/2024.
//

#include "AssetDescriptor.h"
#include "AssetTool.h"

namespace AssetTool {
    using namespace Helpers;

    IProperties::~IProperties()           = default;
    IAssetDescriptor::~IAssetDescriptor() = default;

    void SpriteProperties::Deserialize(ByteArray& bytes) {
        auto offsetPtrs = MemCopy(bytes.data(), &m_Width, sizeof(u32));
        offsetPtrs      = MemCopy(offsetPtrs.SourcePtr, &m_Height, sizeof(u32));
        offsetPtrs      = MemCopy(offsetPtrs.SourcePtr, &m_IsAlpha, sizeof(bool));
        offsetPtrs.Nullify();
    }

    void FontProperties::Deserialize(ByteArray& bytes) {
        auto offsetPtrs = MemCopy(bytes.data(), &m_DefaultSize, sizeof(u32));
        offsetPtrs.Nullify();
    }

    void AudioProperties::Deserialize(ByteArray& bytes) {
        auto offsetPtrs = MemCopy(bytes.data(), &m_SampleCount, sizeof(u32));
        offsetPtrs      = MemCopy(offsetPtrs.SourcePtr, &m_SampleRate, sizeof(u32));
        offsetPtrs      = MemCopy(offsetPtrs.SourcePtr, &m_Channels, sizeof(u32));
        offsetPtrs.Nullify();
    }

    void LevelProperties::Deserialize(ByteArray& bytes) {
        auto offsetPtrs = MemCopy(bytes.data(), &m_Rows, sizeof(u32));
        offsetPtrs      = MemCopy(offsetPtrs.SourcePtr, &m_Columns, sizeof(u32));
        offsetPtrs      = MemCopy(offsetPtrs.SourcePtr, &m_PlayerStart.x, sizeof(f32));
        offsetPtrs      = MemCopy(offsetPtrs.SourcePtr, &m_PlayerStart.y, sizeof(f32));
        offsetPtrs      = MemCopy(offsetPtrs.SourcePtr, &m_ObjectivePosition.x, sizeof(f32));
        offsetPtrs      = MemCopy(offsetPtrs.SourcePtr, &m_ObjectivePosition.y, sizeof(f32));
        offsetPtrs.Nullify();
    }

    ByteArray SpriteProperties::Serialize() {
        ByteArray bytes = {0x0};
        bytes.resize(Size());

        auto offsetPtrs = MemCopy(&m_Width, bytes.data(), sizeof(u32));
        offsetPtrs      = MemCopy(&m_Height, offsetPtrs.DestPtr, sizeof(u32));
        offsetPtrs      = MemCopy(&m_IsAlpha, offsetPtrs.DestPtr, sizeof(bool));
        offsetPtrs.Nullify();

        return bytes;
    }

    ByteArray FontProperties::Serialize() {
        ByteArray bytes = {0x0};
        bytes.resize(Size());

        auto offsetPtrs = MemCopy(&m_DefaultSize, bytes.data(), sizeof(u32));
        offsetPtrs.Nullify();

        return bytes;
    }

    ByteArray AudioProperties::Serialize() {
        ByteArray bytes = {0x0};
        bytes.resize(Size());

        auto offsetPtrs = MemCopy(&m_SampleCount, bytes.data(), sizeof(u32));
        offsetPtrs      = MemCopy(&m_SampleRate, offsetPtrs.DestPtr, sizeof(u32));
        offsetPtrs      = MemCopy(&m_Channels, offsetPtrs.DestPtr, sizeof(u32));
        offsetPtrs.Nullify();

        return bytes;
    }

    ByteArray LevelProperties::Serialize() {
        ByteArray bytes = {0x0};
        bytes.resize(Size());

        auto offsetPtrs = MemCopy(&m_Rows, bytes.data(), sizeof(u32));
        offsetPtrs      = MemCopy(&m_Columns, offsetPtrs.DestPtr, sizeof(u32));
        offsetPtrs      = MemCopy(&m_PlayerStart.x, offsetPtrs.DestPtr, sizeof(f32));
        offsetPtrs      = MemCopy(&m_PlayerStart.y, offsetPtrs.DestPtr, sizeof(f32));
        offsetPtrs      = MemCopy(&m_ObjectivePosition.x, offsetPtrs.DestPtr, sizeof(f32));
        offsetPtrs      = MemCopy(&m_ObjectivePosition.y, offsetPtrs.DestPtr, sizeof(f32));
        offsetPtrs.Nullify();

        return bytes;
    }

    SpriteDescriptor::SpriteDescriptor() {
        this->m_Properties = std::make_unique<SpriteProperties>();
        this->m_Type       = AssetType::Sprite;
    }

    FontDescriptor::FontDescriptor() {
        this->m_Properties = std::make_unique<FontProperties>();
        this->m_Type       = AssetType::Font;
    }

    AudioDescriptor::AudioDescriptor() {
        this->m_Properties = std::make_unique<AudioProperties>();
        this->m_Type       = AssetType::Audio;
    }

    LevelDescriptor::LevelDescriptor() {
        this->m_Properties = std::make_unique<LevelProperties>();
        this->m_Type       = AssetType::Level;
    }

    ByteArray IAssetDescriptor::Serialize() {
        ByteArray bytes = {0x0};
        size_t size     = GetSize();
        bytes.resize(size);

        size_t nameLen       = m_Name.length();
        size_t propsLen      = m_Properties->GetSize();
        size_t dataLen       = m_SrcData.size();
        ByteArray propsBytes = m_Properties->Serialize();

        auto offsetPtrs = MemCopy(&size, bytes.data(), sizeof(size_t));
        offsetPtrs      = MemCopy(&m_Type, offsetPtrs.DestPtr, sizeof(u8));
        offsetPtrs      = MemCopy(&m_Version, offsetPtrs.DestPtr, sizeof(u32));
        offsetPtrs      = MemCopy(&nameLen, offsetPtrs.DestPtr, sizeof(u32));
        // `const_cast` should be safe here as MemCopy does not modify it
        offsetPtrs = MemCopy(const_cast<char*>(m_Name.c_str()), offsetPtrs.DestPtr, nameLen);
        offsetPtrs = MemCopy(&propsLen, offsetPtrs.DestPtr, sizeof(u32));
        offsetPtrs = MemCopy(propsBytes.data(), offsetPtrs.DestPtr, propsLen);
        offsetPtrs = MemCopy(&dataLen, offsetPtrs.DestPtr, sizeof(u32));
        offsetPtrs = MemCopy(m_SrcData.data(), offsetPtrs.DestPtr, dataLen);
        offsetPtrs.Nullify();

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

        auto offsetPtrs = MemCopy(data.data(), &size, sizeof(size_t));
        offsetPtrs      = MemCopy(offsetPtrs.SourcePtr, &type, sizeof(u8));
        offsetPtrs      = MemCopy(offsetPtrs.SourcePtr, &version, sizeof(u32));
        offsetPtrs      = MemCopy(offsetPtrs.SourcePtr, &nameLen, sizeof(u32));

        const auto name = new char[nameLen];
        offsetPtrs      = MemCopy(offsetPtrs.SourcePtr, name, nameLen);
        name[nameLen]   = '\0';

        offsetPtrs = MemCopy(offsetPtrs.SourcePtr, &propsLen, sizeof(u32));
        propertiesBytes.resize(propsLen);

        offsetPtrs = MemCopy(offsetPtrs.SourcePtr, propertiesBytes.data(), propsLen);
        offsetPtrs = MemCopy(offsetPtrs.SourcePtr, &dataLen, sizeof(u32));
        dataBytes.resize(dataLen);

        offsetPtrs = MemCopy(offsetPtrs.SourcePtr, dataBytes.data(), dataLen);
        offsetPtrs.Nullify();

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

        descriptor->m_Properties->Deserialize(propertiesBytes);
        descriptor->m_Name    = nameStr;
        descriptor->m_Version = version;
        descriptor->m_Type    = assetType;
        descriptor->m_SrcData = dataBytes;

        return descriptor;
    }
}  // namespace AssetTool