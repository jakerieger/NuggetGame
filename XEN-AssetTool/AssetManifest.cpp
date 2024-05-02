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
        m_Filename        = manifest["filename"].GetString();
        const auto assets = manifest["assets"].GetArray();
        if (assets.Size() > 0) {
            for (const auto& asset : assets) {
                const auto name       = asset["name"].GetString();
                const auto type       = asset["type"].GetString();
                const auto properties = asset["properties"].GetObject();

                if (strcmp(type, "sprite") == 0) {
                    assert(manifest.HasMember("filename"));
                    const auto filename   = asset["filename"].GetString();
                    const auto descriptor = new SpriteDescriptor;
                    descriptor->m_Name    = name;
                    descriptor->m_Version = m_Version;
                    auto srcBytes         = IO::ReadAllBytes(filename);
                    if (!srcBytes.has_value())
                        break;
                    descriptor->m_SrcData                  = srcBytes.value();
                    descriptor->GetProperties()->m_Height  = properties["height"].GetInt();
                    descriptor->GetProperties()->m_Width   = properties["width"].GetInt();
                    descriptor->GetProperties()->m_IsAlpha = properties["alpha"].GetBool();

                    m_Descriptors.push_back(descriptor);
                }

                if (strcmp(type, "font") == 0) {
                    assert(manifest.HasMember("filename"));
                    const auto filename   = asset["filename"].GetString();
                    const auto descriptor = new FontDescriptor;
                    descriptor->m_Name    = name;
                    descriptor->m_Version = m_Version;
                    auto srcBytes         = IO::ReadAllBytes(filename);
                    if (!srcBytes.has_value())
                        break;
                    descriptor->m_SrcData                      = srcBytes.value();
                    descriptor->GetProperties()->m_DefaultSize = properties["defaultSize"].GetInt();

                    m_Descriptors.push_back(descriptor);
                }

                if (strcmp(type, "audio") == 0) {
                    assert(manifest.HasMember("filename"));
                    const auto filename   = asset["filename"].GetString();
                    const auto descriptor = new AudioDescriptor;
                    descriptor->m_Name    = name;
                    descriptor->m_Version = m_Version;
                    auto srcBytes         = IO::ReadAllBytes(filename);
                    if (!srcBytes.has_value())
                        break;
                    descriptor->m_SrcData                      = srcBytes.value();
                    descriptor->GetProperties()->m_SampleCount = properties["sampleCount"].GetInt();
                    descriptor->GetProperties()->m_SampleRate  = properties["sampleRate"].GetInt();
                    descriptor->GetProperties()->m_Channels    = properties["channels"].GetInt();

                    m_Descriptors.push_back(descriptor);
                }

                if (strcmp(type, "level") == 0) {
                    const auto descriptor = new LevelDescriptor;
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

                    descriptor->GetProperties()->m_Rows        = properties["rows"].GetInt();
                    descriptor->GetProperties()->m_Columns     = properties["columns"].GetInt();
                    descriptor->GetProperties()->m_PlayerStart = {
                      properties["playerStart"]["x"].GetFloat(),
                      properties["playerStart"]["y"].GetFloat(),
                    };
                    descriptor->GetProperties()->m_ObjectivePosition = {
                      properties["objectivePosition"]["x"].GetFloat(),
                      properties["objectivePosition"]["y"].GetFloat(),
                    };

                    m_Descriptors.push_back(descriptor);
                }
            }
        }
    }

    AssetManifest::AssetManifest(const std::filesystem::path& manifest) {
        const auto readResult = IO::Read(manifest);
        if (!readResult.has_value()) {
            throw std::runtime_error("Failed to read manifest file");
        }

        const std::string& manifestSource = readResult.value();
        Document manifestDocument;
        manifestDocument.Parse(manifestSource.c_str());

        if (!ValidateManifest(manifestDocument)) {
            throw std::runtime_error("Failed to validate manifest file");
        }

        ParseManifest(manifestDocument);
    }

    AssetManifest::~AssetManifest() {
        if (!m_Descriptors.empty()) {
            for (const auto& descriptor : m_Descriptors) {
                delete descriptor;
            }
        }

        m_Descriptors.clear();
    }

    std::optional<std::vector<u8>> AssetManifest::Serialize() {
        std::vector<u8> bytes;

        int reserveSize      = std::accumulate(m_Descriptors.begin(),
                                          m_Descriptors.end(),
                                          0,
                                          [](int acc, const IAssetDescriptor* descriptor) {
                                              return acc + (int)descriptor->GetSize();
                                          });
        const u32 assetCount = m_Descriptors.size();
        reserveSize += sizeof(u32);
        bytes.resize((size_t)reserveSize);

        memcpy(bytes.data(), &assetCount, sizeof(u32));
        size_t offset = sizeof(u32);

        for (const auto& descriptor : m_Descriptors) {
            auto descriptorBytes   = descriptor->Serialize();
            const auto offsetDelta = descriptor->GetSize();
            memcpy(bytes.data() + offset, descriptorBytes.data(), offsetDelta);
            offset += offsetDelta;
        }

        return bytes;
    }

    std::optional<AssetManifest> AssetManifest::Deserialize() {
        return std::nullopt;
    }

    size_t AssetManifest::GetSize() {
        const int size = std::accumulate(m_Descriptors.begin(),
                                         m_Descriptors.end(),
                                         0,
                                         [](int acc, const IAssetDescriptor* descriptor) {
                                             return acc + (int)descriptor->GetSize();
                                         });
        return (size_t)size;
    }
}  // namespace AssetTool