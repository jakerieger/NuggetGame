// Author: Jake Rieger
// Created: 4/30/2024.
//

#include "AssetDescriptor.h"

namespace AssetTool {
    IProperties::~IProperties() = default;

    IProperties* SpriteProperties::Deserialize() {
        return nullptr;
    }

    std::vector<u8> AudioProperties::Serialize() {
        std::vector<u8> bytes = {};
        bytes.resize(GetSize());

        memcpy(bytes.data(), &m_SampleCount, sizeof(u32));
        memcpy(bytes.data() + sizeof(u32), &m_SampleRate, sizeof(u32));
        memcpy(bytes.data() + (sizeof(u32) * 2), &m_Channels, sizeof(u32));

        return bytes;
    }

    IProperties* AudioProperties::Deserialize() {
        return nullptr;
    }

    size_t LevelProperties::GetSize() {
        return sizeof(m_Rows) + sizeof(m_Columns) + (sizeof(f32) * 4);
    }

    std::vector<u8> LevelProperties::Serialize() {
        std::vector<u8> bytes = {};
        bytes.resize(GetSize());

        memcpy(bytes.data(), &m_Rows, sizeof(m_Rows));
        memcpy(bytes.data() + sizeof(m_Rows), &m_Columns, sizeof(m_Columns));

        const auto playerX = m_PlayerStart.x;
        const auto playerY = m_PlayerStart.y;
        const auto objX    = m_ObjectivePosition.x;
        const auto objY    = m_ObjectivePosition.y;

        memcpy(bytes.data() + sizeof(m_Rows) + sizeof(m_Columns), &playerX, sizeof(f32));
        memcpy(bytes.data() + sizeof(m_Rows) + sizeof(m_Columns) + sizeof(f32),
               &playerY,
               sizeof(f32));

        memcpy(bytes.data() + sizeof(m_Rows) + sizeof(m_Columns) + sizeof(f32) + sizeof(f32),
               &objX,
               sizeof(f32));
        memcpy(bytes.data() + sizeof(m_Rows) + sizeof(m_Columns) + sizeof(f32) + sizeof(f32) +
               sizeof(f32),
               &objY,
               sizeof(f32));

        return bytes;
    }

    IProperties* LevelProperties::Deserialize() {
        return nullptr;
    }

    size_t IAssetDescriptor::GetSize() const {
        return sizeof(m_Type) + m_Name.size() + sizeof(m_Version) + m_SrcData.size() +
               m_Properties->GetSize() + (sizeof(u32) * 3) -
               3;  // Why subtract 3? I have no fucking idea, something to do with byte
        // alignment or some shit
    }

    IAssetDescriptor::~IAssetDescriptor() {
        delete m_Properties;
    }

    SpriteDescriptor::SpriteDescriptor() {
        this->m_Properties = new SpriteProperties;
        this->m_Type       = AssetType::Sprite;
    }

    IAssetDescriptor* SpriteDescriptor::Deserialize(const std::vector<u8>& data) {
        return nullptr;
    }

    SpriteProperties* SpriteDescriptor::GetProperties() const {
        return dynamic_cast<SpriteProperties*>(m_Properties);
    }

    FontDescriptor::FontDescriptor() {
        this->m_Properties = new FontProperties;
        this->m_Type       = AssetType::Font;
    }

    IAssetDescriptor* FontDescriptor::Deserialize(const std::vector<u8>& data) {
        return nullptr;
    }

    FontProperties* FontDescriptor::GetProperties() const {
        return dynamic_cast<FontProperties*>(m_Properties);
    }

    AudioDescriptor::AudioDescriptor() {
        this->m_Properties = new AudioProperties;
        this->m_Type       = AssetType::Audio;
    }

    IAssetDescriptor* AudioDescriptor::Deserialize(const std::vector<u8>& data) {
        return nullptr;
    }

    AudioProperties* AudioDescriptor::GetProperties() const {
        return dynamic_cast<AudioProperties*>(m_Properties);
    }

    LevelDescriptor::LevelDescriptor() {
        this->m_Properties = new LevelProperties;
        this->m_Type       = AssetType::Level;
    }

    IAssetDescriptor* LevelDescriptor::Deserialize(const std::vector<u8>& data) {
        return nullptr;
    }

    LevelProperties* LevelDescriptor::GetProperties() const {
        return dynamic_cast<LevelProperties*>(m_Properties);
    }

    size_t SpriteProperties::GetSize() {
        return (sizeof(u32) * 2) + sizeof(bool);
    }

    std::vector<u8> SpriteProperties::Serialize() {
        std::vector<u8> bytes = {};
        bytes.resize(GetSize());

        memcpy(bytes.data(), &m_Width, sizeof(u32));
        memcpy(bytes.data() + sizeof(u32), &m_Height, sizeof(u32));
        memcpy(bytes.data() + (sizeof(u32) * 2), &m_IsAlpha, sizeof(bool));

        return bytes;
    }

    size_t FontProperties::GetSize() {
        return sizeof(m_DefaultSize);
    }

    std::vector<u8> FontProperties::Serialize() {
        std::vector<u8> bytes = {};
        bytes.resize(GetSize());
        memcpy(bytes.data(), &m_DefaultSize, sizeof(m_DefaultSize));
        return bytes;
    }

    IProperties* FontProperties::Deserialize() {
        return nullptr;
    }

    size_t AudioProperties::GetSize() {
        return sizeof(u32) * 3;
    }

    std::vector<u8> IAssetDescriptor::Serialize() {
        printf("Processing Asset Descriptor '%s'. ", m_Name.c_str());

        std::vector<u8> bytes = {};

        const auto type            = (u8)m_Type;
        const auto version         = m_Version;
        const auto nameLen         = (u32)m_Name.size();
        const auto propertiesLen   = (u32)m_Properties->GetSize();
        const auto propertiesBytes = m_Properties->Serialize();
        const auto dataLen         = (u32)m_SrcData.size();
        const auto data            = m_SrcData.data();

        printf("Size: %llu bytes.\n", m_SrcData.size());

        const auto reserveSize = GetSize();
        bytes.resize(reserveSize);

        memcpy(bytes.data(), &type, sizeof(u8));
        memcpy(bytes.data() + sizeof(type), &version, sizeof(u32));
        memcpy(bytes.data() + sizeof(type) + sizeof(version), &nameLen, sizeof(u32));
        memcpy(bytes.data() + sizeof(type) + sizeof(version) + sizeof(nameLen),
               m_Name.c_str(),
               nameLen);
        memcpy(bytes.data() + sizeof(type) + sizeof(version) + sizeof(nameLen) + nameLen,
               &propertiesLen,
               sizeof(u32));
        memcpy(bytes.data() + sizeof(type) + sizeof(version) + sizeof(nameLen) + nameLen +
                 sizeof(propertiesLen),
               propertiesBytes.data(),
               propertiesBytes.size());
        memcpy(bytes.data() + sizeof(type) + sizeof(version) + sizeof(nameLen) + nameLen +
                 sizeof(propertiesLen) + propertiesLen,
               &dataLen,
               sizeof(u32));
        memcpy(bytes.data() + sizeof(type) + sizeof(version) + sizeof(nameLen) + nameLen +
                 sizeof(propertiesLen) + propertiesLen + sizeof(dataLen),
               data,
               m_SrcData.size());

        printf("Descriptor serialized.\n");

        return bytes;
    }

}  // namespace AssetTool