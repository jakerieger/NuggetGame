// Author: Jake Rieger
// Created: 5/1/2024.
//

#include "AssetManifest.h"
#include "PlatformTools.h"

#include <numeric>

namespace AssetTool {
    using namespace PlatformTools;
    using namespace rapidjson;

    static bool ValidateManifest(const Document& manifest) {
        if (!manifest.IsObject())
            return false;
        if (!manifest.HasMember("version") || !manifest["version"].IsNumber())
            return false;
        if (!manifest.HasMember("filename") || !manifest["filename"].IsString())
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
                const auto filename   = asset["filename"].GetString();
                const auto properties = asset["properties"].GetObject();

                if (strcmp(type, "sprite") == 0) {
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

                if (strcmp(type, "font") == 0) {}

                if (strcmp(type, "audio") == 0) {}

                if (strcmp(type, "level") == 0) {}
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

    void AssetManifest::Serialize() const {
        std::vector<u8> bytes;
        const size_t reserveSize =
          std::accumulate(m_Descriptors.begin(),
                          m_Descriptors.end(),
                          0,
                          [](const size_t acc, const IAssetDescriptor* descriptor) {
                              return acc + descriptor->GetSize();
                          });
        bytes.resize(reserveSize);

        size_t offset = 0;
        for (const auto& descriptor : m_Descriptors) {
            auto descriptorBytes   = descriptor->Serialize();
            const auto offsetDelta = descriptor->GetSize();
            memcpy(bytes.data() + offset, descriptorBytes.data(), descriptorBytes.size());
            offset += offsetDelta;
        }

        if (!IO::WriteAllBytes(m_Filename, bytes)) {
            throw std::runtime_error("Failed to serialize asset manifest");
        }
    }

    std::optional<AssetManifest> AssetManifest::Deserialize() {
        return std::nullopt;
    }
}  // namespace AssetTool