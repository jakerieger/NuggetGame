// Author: Jake Rieger
// Created: 4/30/2024.
//

#pragma once

#include "AssetTool.h"

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
        virtual size_t GetSize()                   = 0;
        virtual ByteArray Serialize()              = 0;
        virtual void Deserialize(ByteArray& bytes) = 0;
        virtual ~IProperties();
    };

    class SpriteProperties final : public IProperties {
    public:
        u32 m_Width    = 0;
        u32 m_Height   = 0;
        bool m_IsAlpha = false;

        size_t GetSize() override {
            return Helpers::SizeOfAll<u32, u32, bool>();
        }

        ByteArray Serialize() override;
        void Deserialize(ByteArray& bytes) override;
    };

    class FontProperties final : public IProperties {
    public:
        u32 m_DefaultSize = 0;

        size_t GetSize() override {
            return sizeof(u32);
        }

        ByteArray Serialize() override;
        void Deserialize(ByteArray& bytes) override;
    };

    class AudioProperties final : public IProperties {
    public:
        u32 m_SampleCount = 0;
        u32 m_SampleRate  = 0;
        u32 m_Channels    = 0;

        size_t GetSize() override {
            return Helpers::SizeOfAll<u32, u32, u32>();
        }

        ByteArray Serialize() override;
        void Deserialize(ByteArray& bytes) override;
    };

    class LevelProperties final : public IProperties {
    public:
        u32 m_Rows                           = 0;
        u32 m_Columns                        = 0;
        glm::vec<2, f32> m_PlayerStart       = {};
        glm::vec<2, f32> m_ObjectivePosition = {};

        size_t GetSize() override {
            return Helpers::SizeOfAll<u32, u32, f32, f32, f32, f32>();
        }

        ByteArray Serialize() override;
        void Deserialize(ByteArray& bytes) override;
    };

    class IAssetDescriptor {
    public:
        AssetType m_Type = AssetType::Sprite;
        std::string m_Name;
        u32 m_Version = 0;
        ByteArray m_SrcData;
        std::unique_ptr<IProperties> m_Properties;

        [[nodiscard]] ByteArray Serialize();

        [[nodiscard]] size_t GetSize() const {
            const size_t baseSize              = Helpers::SizeOfAll<u32, u8, size_t>();
            const size_t propertiesSize        = m_Properties->GetSize();
            const size_t dataSize              = m_SrcData.size();
            const size_t nameSize              = m_Name.size();
            constexpr size_t nameLenSize       = sizeof(u32);
            constexpr size_t propertiesLenSize = sizeof(u32);
            constexpr size_t dataLenSize       = sizeof(u32);

            return baseSize + propertiesSize + dataSize + nameSize + nameLenSize + dataLenSize +
                   propertiesLenSize;
        }

        template<typename T>
        T* As() {
            return dynamic_cast<T*>(this);
        }

        template<typename T>
        T* GetProperties() {
            return dynamic_cast<T*>(m_Properties.get());
        }

        virtual ~IAssetDescriptor();
    };

    class SpriteDescriptor final : public IAssetDescriptor {
    public:
        SpriteDescriptor();
    };

    class FontDescriptor final : public IAssetDescriptor {
    public:
        FontDescriptor();
    };

    class AudioDescriptor final : public IAssetDescriptor {
    public:
        AudioDescriptor();
    };

    class LevelDescriptor final : public IAssetDescriptor {
    public:
        LevelDescriptor();
    };

    namespace AssetDescriptor {
        std::unique_ptr<IAssetDescriptor> Deserialize(ByteArray& data);
    }  // namespace AssetDescriptor
}  // namespace AssetTool