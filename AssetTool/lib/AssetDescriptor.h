// Author: Jake Rieger
// Created: 4/30/2024.
//

#pragma once

#include <string>
#include <vector>

namespace AssetTool {
    enum class AssetType {
        Sprite,
        Font,
        Audio,
        Level,
    };

    class IProperties {
    public:
        virtual size_t GetSize()                       = 0;
        virtual std::vector<unsigned char> Serialize() = 0;
        virtual IProperties* Deserialize()             = 0;
        virtual ~IProperties()                         = default;
    };

    class SpriteProperties final : public IProperties {
    public:
        unsigned int m_Width  = 0;
        unsigned int m_Height = 0;
        bool m_IsAlpha        = false;

        size_t GetSize() override {
            return (sizeof(unsigned int) * 2) + sizeof(bool);
        }

        std::vector<unsigned char> Serialize() override {
            std::vector<unsigned char> bytes = {};
            bytes.resize(GetSize());

            memcpy(bytes.data(), &m_Width, sizeof(unsigned int));
            memcpy(bytes.data() + sizeof(unsigned int), &m_Height, sizeof(unsigned int));
            memcpy(bytes.data() + sizeof(unsigned int) * 2, &m_IsAlpha, sizeof(bool));

            return bytes;
        }

        IProperties* Deserialize() override {
            return this;
        }
    };

    class AudioProperties final : public IProperties {};

    class LevelProperties final : public IProperties {};

    class IAssetDescriptor {
    public:
        AssetType m_Type;
        std::string m_Name;
        unsigned int m_Version;
        std::vector<unsigned char> m_SrcData;
        IProperties* m_Properties;

        virtual std::vector<unsigned char> Serialize()                                = 0;
        virtual IAssetDescriptor* Deserialize(const std::vector<unsigned char>& data) = 0;

        virtual ~IAssetDescriptor() {
            delete m_Properties;
        }
    };

    class SpriteDescriptor final : public IAssetDescriptor {
    public:
        SpriteDescriptor() {
            this->m_Properties = new SpriteProperties;
        }

        std::vector<unsigned char> Serialize() override {
            return {};
        }

        IAssetDescriptor* Deserialize(const std::vector<unsigned char>& data) override {
            return this;
        }
    };

    class FontDescriptor final : public IAssetDescriptor {};
    class AudioDescriptor final : public IAssetDescriptor {};
    class LevelDescriptor final : public IAssetDescriptor {};
}  // namespace AssetTool