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
        virtual ~IProperties();
    };

    class SpriteProperties final : public IProperties {
    public:
        u32 m_Width    = 0;
        u32 m_Height   = 0;
        bool m_IsAlpha = false;

        size_t GetSize() override;
        std::vector<u8> Serialize() override;
        IProperties* Deserialize() override;
    };

    class FontProperties final : public IProperties {
    public:
        u32 m_DefaultSize = 0;

        size_t GetSize() override;
        std::vector<u8> Serialize() override;
        IProperties* Deserialize() override;
    };

    class AudioProperties final : public IProperties {
    public:
        u32 m_SampleCount = 0;
        u32 m_SampleRate  = 0;
        u32 m_Channels    = 0;

        size_t GetSize() override;
        std::vector<u8> Serialize() override;
        IProperties* Deserialize() override;
    };

    class LevelProperties final : public IProperties {
    public:
        u32 m_Rows                           = 0;
        u32 m_Columns                        = 0;
        glm::vec<2, f32> m_PlayerStart       = {};
        glm::vec<2, f32> m_ObjectivePosition = {};

        size_t GetSize() override;
        std::vector<u8> Serialize() override;
        IProperties* Deserialize() override;
    };

    class IAssetDescriptor {
    public:
        AssetType m_Type = AssetType::Sprite;
        std::string m_Name;
        u32 m_Version = 0;
        std::vector<u8> m_SrcData;
        IProperties* m_Properties {};

        std::vector<u8> Serialize();

        template <typename T>
        static T* Deserialize(const std::vector<u8>& data) {
            static_assert(std::is_base_of_v<IAssetDescriptor, T>, "T must be subclass of IAssetDescriptor");

            return nullptr;
        }

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
}  // namespace AssetTool