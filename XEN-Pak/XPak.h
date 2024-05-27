#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <filesystem>
#include <yaml-cpp/yaml.h>
#include <AudioFile.h>

namespace CRC32 {
    static uint32_t CRC32_TABLE[256];

    void GenerateTable();

    uint32_t CRC32(const uint8_t* data, size_t length);
}  // namespace CRC32

namespace XPak {
    using std::vector;

    constexpr size_t HEADER_SIZE  = 16;
    constexpr size_t ENTRY_SIZE   = 140;
    constexpr size_t MAX_PATH_LEN = 128;

    struct FPakHeader {
        FPakHeader();
        FPakHeader(uint16_t versionMajor,
                   uint16_t versionMinor,
                   uint32_t checksum,
                   uint32_t numEntries);

        const uint8_t Magic[4] = {'X', 'P', 'A', 'K'};
        uint16_t VersionMajor  = 0;
        uint16_t VersionMinor  = 0;
        uint32_t Checksum      = 0;
        uint32_t NumEntries    = 0;

        [[nodiscard]] vector<uint8_t> Serialize() const;

        static std::unique_ptr<FPakHeader> Deserialize(vector<uint8_t>& bytes);
    };

    struct FTableEntry {
        FTableEntry();
        FTableEntry(const char* path, uint32_t blockOffset);

        char Path[128]       = {};
        uint32_t BlockOffset = 0;
        size_t BlockSize     = 0;

        [[nodiscard]] vector<uint8_t> Serialize() const;

        static FTableEntry Deserialize(const vector<uint8_t>& bytes);
    };

    struct FXPak {
        FPakHeader* Header = nullptr;
        vector<FTableEntry> TableOfContents;
        vector<uint8_t> DataBlocks;

        [[nodiscard]] vector<uint8_t> Serialize() const;

        static std::unique_ptr<FXPak> Deserialize();
    };

    vector<uint8_t> Compress(const vector<uint8_t>& bytes);

    vector<uint8_t> Decompress(const vector<uint8_t>& bytes);
}  // namespace XPak

namespace XMeta {
    using std::vector;

    constexpr size_t META_SIZE = 256;

    std::string GenerateGUID();

    enum class EAssetType : uint8_t {
        BITMAP  = 0,
        AUDIO   = 1,
        FONT    = 2,
        UNKNOWN = 3,
    };

    enum class EBitmapFormat : uint8_t {
        RGB,
        RGBA,
        ARGB,
    };

    enum class EAudioFormat : uint8_t {
        FLOAT,
        INT,
        UINT,
    };

    enum class EFontType : uint8_t {
        TRUE_TYPE,
        OPEN_TYPE,
    };

    enum class EFontFamily : uint8_t {
        MONOSPACE,
        SERIF,
        SANS_SERIF,
        DISPLAY,
    };

    struct FAssetProps {
        virtual void ReadProperties(const YAML::Node& node) = 0;
        virtual ~FAssetProps();
        constexpr virtual size_t GetSize() = 0;
    };

    struct FBitmapProperties final : FAssetProps {
        uint32_t Width       = 0;
        uint32_t Height      = 0;
        EBitmapFormat Format = EBitmapFormat::RGB;

        void ReadProperties(const YAML::Node& node) override;

        constexpr size_t GetSize() override;
    };

    struct FAudioProperties final : FAssetProps {
        uint32_t Channels     = 0;
        EAudioFormat Encoding = EAudioFormat::FLOAT;
        uint32_t BitSize      = 0;

        void ReadProperties(const YAML::Node& node) override;

        constexpr size_t GetSize() override;
    };

    struct FFontProperties final : FAssetProps {
        EFontType Type     = EFontType::TRUE_TYPE;
        EFontFamily Family = EFontFamily::DISPLAY;

        void ReadProperties(const YAML::Node& node) override;

        constexpr size_t GetSize() override;
    };

    /// @brief XAsset metadata container.
    /// @tparam PropType Asset properties type. Must be child of `FAssetProps`.
    template<typename PropType>
    struct FAssetMetadata {
        FAssetMetadata();
        std::string GUID;
        uint32_t Version = 0;
        PropType Properties;
        YAML::Node YamlNode = {};
    };

    /// @brief Reads asset metadata from disk or from YAML string.
    /// @tparam PropType Asset properties type. Must be child of `FAssetProps`.
    /// @param data Either the YAML data as a string or a path to the .xasset file on disk.
    /// @param isFile If true, will read from data as filepath. Default is false.
    /// @return
    template<typename PropType>
    FAssetMetadata<PropType> ReadAsset(const std::string& data, bool isFile = false);

    template<typename PropType>
    struct FXMeta {
        FXMeta();
        vector<FAssetMetadata<PropType>> Assets;

        vector<uint8_t> Serialize();

        static FXMeta<PropType> Deserialize(const vector<uint8_t>& bytes);

        void Print();
    };

    template<typename PropType>
    void Print(const std::string& path);
}  // namespace XMeta

namespace XPak {
    namespace fs = std::filesystem;
    using namespace XMeta;

    namespace Helpers {
        std::unordered_map<fs::path, fs::path> GetPakAndMetaFiles(const fs::path& dir);

        template<typename T>
        std::vector<T> InterleaveVectors(const std::vector<T>& vec1, const std::vector<T>& vec2);

        int32_t MakeMultiple(int32_t number, int32_t multiple);

        std::vector<std::string> CopyNodeToVector(const YAML::Node& manifest,
                                                  const std::string& name);

        std::vector<uint8_t> LoadBitmapToVector(
          const std::string& asset, int& x, int& y, int& nrChannels, int desiredChannels = 4);

        template<typename T>
        void LoadSamples(const std::string& filename, vector<T>& samplesOut);

        template<typename T>
        vector<uint8_t> SamplesToBytes(vector<T>& samplesIn);
    }  // namespace Helpers

    void ProcessBitmap(const std::string& asset,
                       FAssetMetadata<FBitmapProperties>& meta,
                       vector<uint8_t>& bytesOut);

    void ProcessAudio(const std::string& asset,
                      FAssetMetadata<FAudioProperties>& meta,
                      vector<uint8_t>& bytesOut);

    void ProcessFont(const std::string& asset,
                     FAssetMetadata<FFontProperties>& meta,
                     vector<uint8_t>& bytesOut);

    template<typename PropType>
    void WritePak(const vector<std::string>& assets,
                  const fs::path& outDir,
                  const std::string& name,
                  FXPak& pakOut,
                  FXMeta<PropType>& metaOut);

    void Pack(const YAML::Node& manifest, const fs::path& outDir);

    struct FXAsset {
        FTableEntry Entry;
        std::string PakFile;
    };

    vector<FXAsset> Unpack(const fs::path& pakFile);

    vector<FXAsset> UnpackAll(const fs::path& dataDir);

    vector<uint8_t> FetchAsset(const std::string& path, const vector<FXAsset>& assets);

    vector<uint8_t> FetchAssetFromDisk(const std::string& path);
}  // namespace XPak