// Author: Jake Rieger
// Created: 4/30/2024.
//

#pragma once

#include <string>
#include <vector>
#include <STL.h>
#include <glm/vec2.hpp>

namespace AssetTool {
    enum class AssetType {
        Sprite,
        Font,
        Audio,
        Level,
    };

    class IProperties {
    public:
        virtual size_t GetSize()            = 0;
        virtual std::vector<u8> Serialize() = 0;
        virtual IProperties* Deserialize()  = 0;
        virtual ~IProperties()              = default;
    };

    class SpriteProperties final : public IProperties {
    public:
        u32 m_Width    = 0;
        u32 m_Height   = 0;
        bool m_IsAlpha = false;

        size_t GetSize() override {
            return (sizeof(u32) * 2) + sizeof(bool);
        }

        std::vector<u8> Serialize() override {
            std::vector<u8> bytes = {};
            bytes.resize(GetSize());

            memcpy(bytes.data(), &m_Width, sizeof(u32));
            memcpy(bytes.data() + sizeof(u32), &m_Height, sizeof(u32));
            memcpy(bytes.data() + (sizeof(u32) * 2), &m_IsAlpha, sizeof(bool));

            return bytes;
        }

        IProperties* Deserialize() override {
            return nullptr;
        }
    };

    class FontProperties final : public IProperties {
    public:
        u32 m_DefaultSize = 0;

        size_t GetSize() override {
            return sizeof(m_DefaultSize);
        }

        std::vector<u8> Serialize() override {
            std::vector<u8> bytes = {};
            bytes.resize(GetSize());
            memcpy(bytes.data(), &m_DefaultSize, sizeof(m_DefaultSize));
            return bytes;
        }

        IProperties* Deserialize() override {
            return nullptr;
        }
    };

    class AudioProperties final : public IProperties {
    public:
        u32 m_SampleCount = 0;
        u32 m_SampleRate  = 0;
        u32 m_Channels    = 0;

        size_t GetSize() override {
            return sizeof(u32) * 3;
        }

        std::vector<u8> Serialize() override {
            std::vector<u8> bytes = {};
            bytes.resize(GetSize());

            memcpy(bytes.data(), &m_SampleCount, sizeof(u32));
            memcpy(bytes.data() + sizeof(u32), &m_SampleRate, sizeof(u32));
            memcpy(bytes.data() + (sizeof(u32) * 2), &m_Channels, sizeof(u32));

            return bytes;
        }

        IProperties* Deserialize() override {
            return nullptr;
        }
    };

    class LevelProperties final : public IProperties {
    public:
        u32 m_Rows                           = 0;
        u32 m_Columns                        = 0;
        glm::vec<2, f32> m_PlayerStart       = {};
        glm::vec<2, f32> m_ObjectivePosition = {};

        size_t GetSize() override {
            return sizeof(m_Rows) + sizeof(m_Columns) + (sizeof(f32) * 4);
        }

        std::vector<u8> Serialize() override {
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

        IProperties* Deserialize() override {
            return nullptr;
        }
    };

    class IAssetDescriptor {
    public:
        AssetType m_Type = AssetType::Sprite;
        std::string m_Name;
        u32 m_Version = 0;
        std::vector<u8> m_SrcData;
        IProperties* m_Properties {};

        std::vector<u8> Serialize();
        virtual IAssetDescriptor* Deserialize(const std::vector<u8>& data) = 0;

        [[nodiscard]] size_t GetSize() const {
            return sizeof(m_Type) + m_Name.size() + sizeof(m_Version) + m_SrcData.size() +
                   m_Properties->GetSize() + (sizeof(u32) * 3) -
                   3;  // Why subtract 3? I have no fucking idea, something to do with byte
                       // alignment or some shit
        }

        virtual ~IAssetDescriptor() {
            delete m_Properties;
        }
    };

    class SpriteDescriptor final : public IAssetDescriptor {
    public:
        SpriteDescriptor() {
            this->m_Properties = new SpriteProperties;
            this->m_Type       = AssetType::Sprite;
        }

        IAssetDescriptor* Deserialize(const std::vector<u8>& data) override {
            return nullptr;
        }

        [[nodiscard]] SpriteProperties* GetProperties() const {
            return dynamic_cast<SpriteProperties*>(m_Properties);
        }
    };

    class FontDescriptor final : public IAssetDescriptor {
    public:
        FontDescriptor() {
            this->m_Properties = new FontProperties;
            this->m_Type       = AssetType::Font;
        }

        IAssetDescriptor* Deserialize(const std::vector<u8>& data) override {
            return nullptr;
        }

        [[nodiscard]] FontProperties* GetProperties() const {
            return dynamic_cast<FontProperties*>(m_Properties);
        }
    };

    class AudioDescriptor final : public IAssetDescriptor {
    public:
        AudioDescriptor() {
            this->m_Properties = new AudioProperties;
            this->m_Type       = AssetType::Audio;
        }

        IAssetDescriptor* Deserialize(const std::vector<u8>& data) override {
            return nullptr;
        }

        [[nodiscard]] AudioProperties* GetProperties() const {
            return dynamic_cast<AudioProperties*>(m_Properties);
        }
    };

    class LevelDescriptor final : public IAssetDescriptor {
    public:
        LevelDescriptor() {
            this->m_Properties = new LevelProperties;
            this->m_Type       = AssetType::Level;
        }

        IAssetDescriptor* Deserialize(const std::vector<u8>& data) override {
            return nullptr;
        }

        [[nodiscard]] LevelProperties* GetProperties() const {
            return dynamic_cast<LevelProperties*>(m_Properties);
        }
    };
}  // namespace AssetTool