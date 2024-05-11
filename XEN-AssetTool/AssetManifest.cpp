// Author: Jake Rieger
// Created: 5/1/2024.
//

#include "AssetManifest.h"
#include "PlatformTools.h"

#include <numeric>
#include <fmt/core.h>

namespace AssetTool {
    using namespace PlatformTools;
    using namespace rapidjson;
    using namespace Helpers;

    static bool ValidateManifest(const Document& manifest) {
        if (!manifest.IsObject())
            return false;
        if (!manifest.HasMember("version") || !manifest["version"].IsNumber())
            return false;
        if (!manifest.HasMember("assets") || !manifest["assets"].IsArray())
            return false;

        return true;
    }

    void AssetManifest::ParseManifest(const Document& manifest) {
        m_Version         = manifest["version"].GetInt();
        m_Name            = manifest["name"].GetString();
        const auto assets = manifest["assets"].GetArray();
        if (assets.Size() > 0) {
            for (const auto& asset : assets) {
                const auto name       = asset["name"].GetString();
                const auto type       = asset["type"].GetString();
                const auto properties = asset["properties"].GetObject();
                std::unique_ptr<IAssetDescriptor> descriptor;

                if (strcmp(type, "sprite") == 0) {
                    assert(manifest.HasMember("filename"));
                    const auto filename   = asset["filename"].GetString();
                    descriptor            = std::make_unique<SpriteDescriptor>();
                    descriptor->m_Name    = name;
                    descriptor->m_Version = m_Version;
                    auto srcBytes         = IO::ReadAllBytes(filename);
                    if (!srcBytes.has_value())
                        break;
                    descriptor->m_SrcData = srcBytes.value();
                    descriptor->GetProperties<SpriteProperties>()->m_Height =
                      properties["height"].GetInt();
                    descriptor->GetProperties<SpriteProperties>()->m_Width =
                      properties["width"].GetInt();
                    descriptor->GetProperties<SpriteProperties>()->m_IsAlpha =
                      properties["alpha"].GetBool();
                }

                if (strcmp(type, "font") == 0) {
                    assert(manifest.HasMember("filename"));
                    const auto filename   = asset["filename"].GetString();
                    descriptor            = std::make_unique<FontDescriptor>();
                    descriptor->m_Name    = name;
                    descriptor->m_Version = m_Version;
                    auto srcBytes         = IO::ReadAllBytes(filename);
                    if (!srcBytes.has_value())
                        break;
                    descriptor->m_SrcData = srcBytes.value();
                    descriptor->GetProperties<FontProperties>()->m_DefaultSize =
                      properties["defaultSize"].GetInt();
                }

                if (strcmp(type, "audio") == 0) {
                    assert(manifest.HasMember("filename"));
                    const auto filename   = asset["filename"].GetString();
                    descriptor            = std::make_unique<AudioDescriptor>();
                    descriptor->m_Name    = name;
                    descriptor->m_Version = m_Version;
                    auto srcBytes         = IO::ReadAllBytes(filename);
                    if (!srcBytes.has_value())
                        break;
                    descriptor->m_SrcData = srcBytes.value();
                    descriptor->GetProperties<AudioProperties>()->m_SampleCount =
                      properties["sampleCount"].GetInt();
                    descriptor->GetProperties<AudioProperties>()->m_SampleRate =
                      properties["sampleRate"].GetInt();
                    descriptor->GetProperties<AudioProperties>()->m_Channels =
                      properties["channels"].GetInt();
                }

                if (strcmp(type, "level") == 0) {
                    descriptor            = std::make_unique<LevelDescriptor>();
                    descriptor->m_Name    = name;
                    descriptor->m_Version = m_Version;
                    auto srcBytes         = asset["data"].GetArray();
                    std::vector<u8> data;
                    std::vector<i32> srcData;
                    for (int i = 0; i < srcBytes.Size(); i++) {
                        auto value = srcBytes[i].GetInt();
                        srcData.push_back(value);
                    }
                    data.resize(sizeof(i32) * srcData.size());
                    std::copy(reinterpret_cast<u8*>(srcData.data()),
                              reinterpret_cast<u8*>(srcData.data() + srcData.size()),
                              data.begin());
                    descriptor->m_SrcData = data;

                    descriptor->GetProperties<LevelProperties>()->m_Rows =
                      properties["rows"].GetInt();
                    descriptor->GetProperties<LevelProperties>()->m_Columns =
                      properties["columns"].GetInt();
                    descriptor->GetProperties<LevelProperties>()->m_PlayerStart = {
                      properties["playerStart"]["x"].GetFloat(),
                      properties["playerStart"]["y"].GetFloat(),
                    };
                    descriptor->GetProperties<LevelProperties>()->m_ObjectivePosition = {
                      properties["objectivePosition"]["x"].GetFloat(),
                      properties["objectivePosition"]["y"].GetFloat(),
                    };
                }

                m_Descriptors.push_back(descriptor);
            }
        }
    }

    AssetManifest::AssetManifest(const std::filesystem::path& manifest) {}

    AssetManifest::~AssetManifest() {}

    ByteArray AssetManifest::Serialize() {
        return {};
    }

    std::unique_ptr<AssetManifest> AssetManifest::Deserialize(const std::vector<u8>& bytes) {
        return nullptr;
    }

    size_t AssetManifest::GetSize() {
        // Size, Name Length, Descriptor Count
        const size_t baseSize = SizeOfAll<size_t, u32, u32>();
        // Number of bytes in Name
        const size_t nameLen = m_Name.length();
        // Combined size of all descriptors
        const int descriptorSize =
          std::accumulate(m_Descriptors.begin(),
                          m_Descriptors.end(),
                          0,
                          [](int acc, const std::unique_ptr<IAssetDescriptor>& descriptor) {
                              return acc + static_cast<int>(descriptor->GetSize());
                          });
        return static_cast<size_t>(descriptorSize) + nameLen + baseSize;
    }
}  // namespace AssetTool