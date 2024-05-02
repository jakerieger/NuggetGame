// Author: Jake Rieger
// Created: 4/30/2024.
//

#pragma once

#include <string>
#include <vector>
#include <STL.h>

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

        std::vector<u8> Serialize() override;

        IProperties* Deserialize() override;
    };

    class AudioProperties final : public IProperties {};

    class LevelProperties final : public IProperties {};

    class IAssetDescriptor {
    public:
        AssetType m_Type;
        std::string m_Name;
        u32 m_Version = 0;
        std::vector<u8> m_SrcData;
        IProperties* m_Properties;

        std::vector<u8> Serialize();
        virtual IAssetDescriptor* Deserialize(const std::vector<u8>& data) = 0;

        [[nodiscard]] size_t GetSize() const {
            return sizeof(m_Type) + m_Name.size() + sizeof(m_Version) + m_SrcData.size() +
                   m_Properties->GetSize();
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

        IAssetDescriptor* Deserialize(const std::vector<u8>& data) override;

        [[nodiscard]] SpriteProperties* GetProperties() const {
            return dynamic_cast<SpriteProperties*>(m_Properties);
        }
    };

    class FontDescriptor final : public IAssetDescriptor {};
    class AudioDescriptor final : public IAssetDescriptor {};
    class LevelDescriptor final : public IAssetDescriptor {};
}  // namespace AssetTool