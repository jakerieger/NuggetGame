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
        virtual size_t GetSize()                         = 0;
        virtual std::vector<u8> Serialize()              = 0;
        virtual void Deserialize(std::vector<u8>& bytes) = 0;
        virtual ~IProperties();
    };

    class SpriteProperties final : public IProperties {
    public:
        u32 m_Width    = 0;
        u32 m_Height   = 0;
        bool m_IsAlpha = false;

        size_t GetSize() override;
        std::vector<u8> Serialize() override;
        void Deserialize(std::vector<u8>& bytes) override;
    };

    class FontProperties final : public IProperties {
    public:
        u32 m_DefaultSize = 0;

        size_t GetSize() override;
        std::vector<u8> Serialize() override;
        void Deserialize(std::vector<u8>& bytes) override;
    };

    class AudioProperties final : public IProperties {
    public:
        u32 m_SampleCount = 0;
        u32 m_SampleRate  = 0;
        u32 m_Channels    = 0;

        size_t GetSize() override;
        std::vector<u8> Serialize() override;
        void Deserialize(std::vector<u8>& bytes) override;
    };

    class LevelProperties final : public IProperties {
    public:
        u32 m_Rows                           = 0;
        u32 m_Columns                        = 0;
        glm::vec<2, f32> m_PlayerStart       = {};
        glm::vec<2, f32> m_ObjectivePosition = {};

        size_t GetSize() override;
        std::vector<u8> Serialize() override;
        void Deserialize(std::vector<u8>& bytes) override;
    };

    class IAssetDescriptor {
    public:
        AssetType m_Type = AssetType::Sprite;
        std::string m_Name;
        u32 m_Version = 0;
        std::vector<u8> m_SrcData;
        IProperties* m_Properties {};

        std::vector<u8> Serialize();

        [[nodiscard]] size_t GetSize() const;
        virtual ~IAssetDescriptor();
    };

    class SpriteDescriptor final : public IAssetDescriptor {
    public:
        SpriteDescriptor();
        [[nodiscard]] SpriteProperties* GetProperties() const;
    };

    class FontDescriptor final : public IAssetDescriptor {
    public:
        FontDescriptor();
        [[nodiscard]] FontProperties* GetProperties() const;
    };

    class AudioDescriptor final : public IAssetDescriptor {
    public:
        AudioDescriptor();
        [[nodiscard]] AudioProperties* GetProperties() const;
    };

    class LevelDescriptor final : public IAssetDescriptor {
    public:
        LevelDescriptor();
        [[nodiscard]] LevelProperties* GetProperties() const;
    };

    namespace AssetDescriptor {
        template<typename T>
        T* Deserialize(std::vector<u8> data) {
            static_assert(std::is_base_of_v<IAssetDescriptor, T>,
                          "T must be subclass of IAssetDescriptor");

            T* out          = new T;
            auto descriptor = dynamic_cast<IAssetDescriptor*>(out);

            memcpy(&descriptor->m_Type, data.data(), sizeof(u8));
            memcpy(&descriptor->m_Version, data.data() + sizeof(u8), sizeof(u32));

            u32 nameLen = 0;
            memcpy(&nameLen, data.data() + sizeof(u8) + sizeof(u32), sizeof(u32));

            const auto name = new char[nameLen];
            strncpy(name, reinterpret_cast<char*>(data.data() + sizeof(u8) + sizeof(u64)), nameLen);
            name[nameLen]      = {'\0'};
            descriptor->m_Name = std::string(name);
            delete name;

            u32 propertiesLen = 0;
            memcpy(&propertiesLen, data.data() + 9 + nameLen, sizeof(u32));

            std::vector<u8> propertiesBytes;
            propertiesBytes.resize(propertiesLen);
            memcpy(propertiesBytes.data(), data.data() + 9 + nameLen + sizeof(u32), propertiesLen);

            descriptor->m_Properties->Deserialize(propertiesBytes);

            u32 dataLen = 0;
            memcpy(&dataLen, data.data() + 9 + nameLen + sizeof(u32) + propertiesLen, sizeof(u32));

            std::vector<u8> dataBytes;
            dataBytes.resize(dataLen);
            memcpy(dataBytes.data(),
                   data.data() + 9 + nameLen + sizeof(u32) + propertiesLen + sizeof(u32),
                   dataLen);
            descriptor->m_SrcData = dataBytes;

            return dynamic_cast<T*>(descriptor);
        }
    }  // namespace AssetDescriptor
}  // namespace AssetTool