// Author: Jake Rieger
// Created: 4/30/2024.
//

#include "AssetDescriptor.h"
#include "PlatformTools.h"
#include "AssetTool.h"

#include <iostream>
#include <numeric>

namespace AssetTool {
    using namespace Utilities;

    IProperties::~IProperties() = default;

    void SpriteProperties::Deserialize(std::vector<u8>& bytes) {
        memcpy(&m_Width, bytes.data(), sizeof(u32));
        memcpy(&m_Height, bytes.data() + SizeOfAll<u32>(), sizeof(u32));
        memcpy(&m_IsAlpha, bytes.data() + SizeOfAll<u64>(), sizeof(bool));
    }

    std::vector<u8> AudioProperties::Serialize() {
        std::vector<u8> bytes = {};
        bytes.resize(GetSize());

        memcpy(bytes.data(), &m_SampleCount, sizeof(u32));
        memcpy(bytes.data() + SizeOfAll<u32>(), &m_SampleRate, sizeof(u32));
        memcpy(bytes.data() + SizeOfAll<u32, u32>(), &m_Channels, sizeof(u32));

        return bytes;
    }

    void AudioProperties::Deserialize(std::vector<u8>& bytes) {
        memcpy(&m_SampleCount, bytes.data(), sizeof(u32));
        memcpy(&m_SampleRate, bytes.data() + SizeOfAll<u32>(), sizeof(u32));
        memcpy(&m_Channels, bytes.data() + SizeOfAll<u32, u32>(), sizeof(u32));
    }

    size_t LevelProperties::GetSize() {
        return sizeof(m_Rows) + sizeof(m_Columns) + (sizeof(f32) * 4);
    }

    std::vector<u8> LevelProperties::Serialize() {
        std::vector<u8> bytes = {};
        bytes.resize(GetSize());

        const auto playerX = m_PlayerStart.x;
        const auto playerY = m_PlayerStart.y;
        const auto objX    = m_ObjectivePosition.x;
        const auto objY    = m_ObjectivePosition.y;

        memcpy(bytes.data(), &m_Rows, sizeof(m_Rows));
        memcpy(bytes.data() + SizeOfAll<u32>(), &m_Columns, sizeof(m_Columns));
        memcpy(bytes.data() + SizeOfAll<u32, u32>(), &playerX, sizeof(f32));
        memcpy(bytes.data() + SizeOfAll<u64, f32>(), &playerY, sizeof(f32));
        memcpy(bytes.data() + SizeOfAll<u64, f64>(), &objX, sizeof(f32));
        memcpy(bytes.data() + SizeOfAll<u64, f64, f32>(), &objY, sizeof(f32));

        return bytes;
    }

    void LevelProperties::Deserialize(std::vector<u8>& bytes) {
        memcpy(&m_Rows, bytes.data(), sizeof(u32));
        memcpy(&m_Columns, bytes.data() + SizeOfAll<u32>(), sizeof(u32));
        memcpy(&m_PlayerStart.x, bytes.data() + SizeOfAll<u64>(), sizeof(f32));
        memcpy(&m_PlayerStart.y, bytes.data() + SizeOfAll<u64, f32>(), sizeof(f32));
        memcpy(&m_ObjectivePosition.x, bytes.data() + SizeOfAll<u64, f64>(), sizeof(f32));
        memcpy(&m_ObjectivePosition.y, bytes.data() + SizeOfAll<u64, f64, f32>(), sizeof(f32));
    }

    size_t IAssetDescriptor::GetSize() const {
        return SizeOfAll<u8, u64, u64>() + m_Name.size() + m_Properties->GetSize() +
               m_SrcData.size();
    }

    IAssetDescriptor::~IAssetDescriptor() {
        delete m_Properties;
    }

    SpriteDescriptor::SpriteDescriptor() {
        this->m_Properties = new SpriteProperties;
        this->m_Type       = AssetType::Sprite;
    }

    SpriteProperties* SpriteDescriptor::GetProperties() const {
        return dynamic_cast<SpriteProperties*>(m_Properties);
    }

    FontDescriptor::FontDescriptor() {
        this->m_Properties = new FontProperties;
        this->m_Type       = AssetType::Font;
    }

    FontProperties* FontDescriptor::GetProperties() const {
        return dynamic_cast<FontProperties*>(m_Properties);
    }

    AudioDescriptor::AudioDescriptor() {
        this->m_Properties = new AudioProperties;
        this->m_Type       = AssetType::Audio;
    }

    AudioProperties* AudioDescriptor::GetProperties() const {
        return dynamic_cast<AudioProperties*>(m_Properties);
    }

    LevelDescriptor::LevelDescriptor() {
        this->m_Properties = new LevelProperties;
        this->m_Type       = AssetType::Level;
    }

    LevelProperties* LevelDescriptor::GetProperties() const {
        return dynamic_cast<LevelProperties*>(m_Properties);
    }

    size_t SpriteProperties::GetSize() {
        return SizeOfAll<u64, bool>();
    }

    std::vector<u8> SpriteProperties::Serialize() {
        std::vector<u8> bytes = {};
        bytes.resize(GetSize());

        memcpy(bytes.data(), &m_Width, sizeof(u32));
        memcpy(bytes.data() + SizeOfAll<u32>(), &m_Height, sizeof(u32));
        memcpy(bytes.data() + SizeOfAll<u64>(), &m_IsAlpha, sizeof(bool));

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

    void FontProperties::Deserialize(std::vector<u8>& bytes) {
        return;
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
        memcpy(bytes.data() + SizeOfAll<u8>(), &version, sizeof(u32));
        memcpy(bytes.data() + SizeOfAll<u8, u32>(), &nameLen, sizeof(u32));
        memcpy(bytes.data() + SizeOfAll<u8, u64>(), m_Name.c_str(), nameLen);
        memcpy(bytes.data() + SizeOfAll<u8, u64>() + nameLen, &propertiesLen, sizeof(u32));
        memcpy(bytes.data() + SizeOfAll<u8, u64, u32>() + nameLen,
               propertiesBytes.data(),
               propertiesBytes.size());
        memcpy(bytes.data() + SizeOfAll<u8, u64, u32>() + nameLen + propertiesLen,
               &dataLen,
               sizeof(u32));
        memcpy(bytes.data() + SizeOfAll<u8, u64, u64>() + nameLen + propertiesLen,
               data,
               m_SrcData.size());

        printf("Descriptor serialized.\n");

        PlatformTools::IO::WriteAllBytes("test.bin", bytes);

        return bytes;
    }

}  // namespace AssetTool