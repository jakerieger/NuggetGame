#include "XPak.h"
#include "IO.h"
#include "stb_image.h"

#include <cassert>
#include <iostream>
#include <numeric>
#include <cstring>
#include <lzma.h>
#include <crossguid/guid.hpp>
#include <ranges>
#include <fmt/format.h>

namespace CRC32 {
    void GenerateTable() {
        constexpr uint32_t polynomial = 0xEDB88320;
        for (uint32_t i = 0; i < 256; i++) {
            uint32_t crc = i;
            for (uint32_t j = 8; j > 0; j--) {
                if (crc & 1) {
                    crc = (crc >> 1) ^ polynomial;
                } else {
                    crc = crc >> 1;
                }
            }
            CRC32_TABLE[i] = crc;
        }
    }

    uint32_t CRC32(const uint8_t* data, const size_t length) {
        uint32_t crc = 0xFFFFFFFF;
        for (size_t i = 0; i < length; i++) {
            crc = (crc >> 8) ^ CRC32_TABLE[(crc ^ data[i]) & 0xFF];
        }
        return crc ^ 0xFFFFFFFF;
    }
}  // namespace CRC32

namespace XPak {
    FPakHeader::FPakHeader() = default;

    FPakHeader::FPakHeader(const uint16_t versionMajor,
                           const uint16_t versionMinor,
                           const uint32_t checksum,
                           const uint32_t numEntries)
        : VersionMajor(versionMajor), VersionMinor(versionMinor), Checksum(checksum),
          NumEntries(numEntries) {}

    std::vector<unsigned char> FPakHeader::Serialize() const {
        vector<uint8_t> bytes(HEADER_SIZE);

        std::memcpy(bytes.data(), Magic, 4);
        std::memcpy(bytes.data() + 4, &VersionMajor, 2);
        std::memcpy(bytes.data() + 6, &VersionMinor, 2);
        std::memcpy(bytes.data() + 8, &Checksum, 4);
        std::memcpy(bytes.data() + 12, &NumEntries, 4);

        return bytes;
    }

    std::unique_ptr<FPakHeader> FPakHeader::Deserialize(vector<uint8_t>& bytes) {
        if (bytes.size() < HEADER_SIZE) {
            std::cerr << "Insufficient data for XPak header.\n";
            exit(-1);
        }

        char magic[5] = {};
        std::memcpy(magic, bytes.data(), 4);
        magic[4] = '\0';
        if (strcmp(magic, "XPAK") != 0) {
            std::cerr << "Invalid XPak header, possible corruption.\n";
            exit(-1);
        }

        auto versionMajor = *reinterpret_cast<uint16_t*>(bytes.data() + 4);
        auto versionMinor = *reinterpret_cast<uint16_t*>(bytes.data() + 6);
        auto checksum     = *reinterpret_cast<uint32_t*>(bytes.data() + 8);
        auto numEntries   = *reinterpret_cast<uint32_t*>(bytes.data() + 12);

        return std::make_unique<FPakHeader>(versionMajor, versionMinor, checksum, numEntries);
    }

    FTableEntry::FTableEntry() = default;
    FTableEntry::FTableEntry(const char* path, const uint32_t blockOffset)
        : BlockOffset(blockOffset) {
        if (strlen(path) > MAX_PATH_LEN) {
            std::cerr << "Path name is too long.\n";
            exit(-1);
        }
        strcpy(Path, path);
    }

    std::vector<unsigned char> FTableEntry::Serialize() const {
        vector<uint8_t> bytes(ENTRY_SIZE);

        std::memcpy(bytes.data(), Path, MAX_PATH_LEN);
        std::memcpy(bytes.data() + MAX_PATH_LEN, &BlockOffset, 4);
        std::memcpy(bytes.data() + MAX_PATH_LEN + 4, &BlockSize, 8);

        return bytes;
    }

    FTableEntry FTableEntry::Deserialize(const vector<uint8_t>& bytes) {
        FTableEntry entry;

        std::memcpy(entry.Path, bytes.data(), MAX_PATH_LEN);
        std::memcpy(&entry.BlockOffset, bytes.data() + MAX_PATH_LEN, 4);
        std::memcpy(&entry.BlockSize, bytes.data() + MAX_PATH_LEN + 4, 8);

        return entry;
    }

    std::vector<unsigned char> FXPak::Serialize() const {
        vector<uint8_t> bytes(HEADER_SIZE + ENTRY_SIZE * TableOfContents.size() +
                              DataBlocks.size());

        auto headerBytes   = Header->Serialize();
        const auto tocSize = ENTRY_SIZE * TableOfContents.size();
        vector<uint8_t> tocBytes(tocSize);
        int i = 0;
        for (auto& entry : TableOfContents) {
            auto entryBytes = entry.Serialize();
            std::memcpy(tocBytes.data() + (i * ENTRY_SIZE), entryBytes.data(), ENTRY_SIZE);
            i++;
        }

        std::memcpy(bytes.data(), headerBytes.data(), HEADER_SIZE);
        std::memcpy(bytes.data() + HEADER_SIZE, tocBytes.data(), tocSize);
        std::memcpy(bytes.data() + HEADER_SIZE + (ENTRY_SIZE * TableOfContents.size()),
                    DataBlocks.data(),
                    DataBlocks.size());

        return bytes;
    }

    std::unique_ptr<FXPak> FXPak::Deserialize() {
        return {};
    }

    std::vector<unsigned char> Compress(const vector<uint8_t>& bytes) {
        vector<uint8_t> compressedBytes;
        const uint8_t* sourceBytes = bytes.data();
        const size_t sourceSize    = bytes.size();

        lzma_stream stream = LZMA_STREAM_INIT;
        lzma_ret ret       = lzma_easy_encoder(&stream, LZMA_PRESET_DEFAULT, LZMA_CHECK_CRC64);

        if (ret != LZMA_OK) {
            std::cerr << "Error initializing the LZMA encoder.\n";
            return {};
        }

        stream.avail_in = sourceSize;
        stream.next_in  = sourceBytes;
        compressedBytes.resize(sourceSize);

        stream.avail_out = sourceSize;
        stream.next_out  = compressedBytes.data();

        ret = lzma_code(&stream, LZMA_FINISH);
        if (ret != LZMA_OK && ret != LZMA_STREAM_END) {
            lzma_end(&stream);
            std::cerr << "Error compressing data..\n";
            return {};
        }

        compressedBytes.resize(compressedBytes.size() - stream.avail_out);
        lzma_end(&stream);

        return compressedBytes;
    }

    std::vector<unsigned char> Decompress(const vector<uint8_t>& bytes) {
        vector<uint8_t> uncompressedBytes;

        lzma_stream stream = LZMA_STREAM_INIT;
        lzma_ret ret       = lzma_stream_decoder(&stream, UINT64_MAX, 0);
        if (ret != LZMA_OK) {
            std::cerr << "Error initializing LZMA decoder.\n";
            return {};
        }

        stream.avail_in = bytes.size();
        stream.next_in  = bytes.data();

        do {
            uint8_t buffer[BUFSIZ];
            stream.avail_out = BUFSIZ;
            stream.next_out  = buffer;
            ret              = lzma_code(&stream, LZMA_RUN);
            if (ret != LZMA_OK && ret != LZMA_STREAM_END) {
                lzma_end(&stream);
                std::cerr << "Error decoding byte in LZMA stream.\n";
                return {};
            }
            uncompressedBytes.insert(uncompressedBytes.end(),
                                     buffer,
                                     buffer + BUFSIZ - stream.avail_out);
        } while (ret != LZMA_STREAM_END);

        uncompressedBytes.resize(stream.total_out);
        lzma_end(&stream);

        return uncompressedBytes;
    }

    std::unordered_map<std::filesystem::path, std::filesystem::path>
    Helpers::GetPakAndMetaFiles(const fs::path& dir) {
        std::unordered_map<fs::path, fs::path> fileMap;

        if (fs::exists(dir) && fs::is_directory(dir)) {
            for (const auto& entry : fs::directory_iterator(dir)) {
                if (entry.is_regular_file() && entry.path().extension() == ".xpak") {
                    const auto& pakFile = entry.path();
                    auto metaFile       = entry.path();
                    metaFile.replace_extension(".xmeta");
                    if (!fs::exists(metaFile)) {
                        std::cerr << "Corresponding metadata file for " << pakFile
                                  << " does not exist.\n";
                        break;
                    }

                    fileMap[pakFile] = metaFile;
                }
            }
        }

        return fileMap;
    }
    template<typename T>
    std::vector<T> Helpers::InterleaveVectors(const std::vector<T>& vec1,
                                              const std::vector<T>& vec2) {
        std::vector<T> result;
        const size_t size = std::min(vec1.size(), vec2.size());

        for (size_t i = 0; i < size; ++i) {
            result.push_back(vec1[i]);
            result.push_back(vec2[i]);
        }

        for (size_t i = size; i < vec1.size(); ++i) {
            result.push_back(vec1[i]);
        }
        for (size_t i = size; i < vec2.size(); ++i) {
            result.push_back(vec2[i]);
        }

        return result;
    }
    int32_t Helpers::MakeMultiple(const int32_t number, const int32_t multiple) {
        const int32_t remainder  = number % multiple;
        const int32_t difference = multiple - remainder;
        return number + difference;
    }
    std::vector<std::string> Helpers::CopyNodeToVector(const YAML::Node& manifest,
                                                       const std::string& name) {
        std::vector<std::string> list;

        if (const auto node = manifest[name]; node.IsSequence()) {
            for (auto&& i : node) {
                list.push_back(i.as<std::string>());
            }
        }

        return list;
    }
    std::vector<uint8_t> Helpers::LoadBitmapToVector(
      const std::string& asset, int& x, int& y, int& nrChannels, const int desiredChannels) {
        uint8_t* bitmapBytes = stbi_load(asset.c_str(), &x, &y, &nrChannels, desiredChannels);

        if (!bitmapBytes) {
            std::cerr << "Failed to load bitmap.\n";
            return {};
        }

        const size_t dataSize = x * y * nrChannels;
        std::vector<uint8_t> bitmapVector(bitmapBytes, bitmapBytes + dataSize);
        stbi_image_free(bitmapBytes);

        return bitmapVector;
    }
    template<typename T>
    void Helpers::LoadSamples(const std::string& filename, vector<T>& samplesOut) {
        AudioFile<T> audioFile;
        if (!audioFile.load(filename)) {
            std::cerr << "Error opening audio file '" << filename << "'\n";
            return;
        }

        samplesOut.reserve(audioFile.getNumSamplesPerChannel() * audioFile.getNumChannels());
        if (audioFile.isMono()) {
            samplesOut = audioFile.samples.at(0);
        } else {
            samplesOut = InterleaveVectors(audioFile.samples.at(0), audioFile.samples.at(1));
        }

        // OpenAL requires that the number of samples be a multiple of the data type size in
        // bytes (ex. float) * the number of channels. If it's not, this line adds additional
        // silence at the end of the buffer to make it a multiple.
        samplesOut.resize(
          MakeMultiple(static_cast<int32_t>(samplesOut.size()),
                       static_cast<int32_t>(sizeof(T) * audioFile.getNumChannels())));
    }
    template<typename T>
    std::vector<unsigned char> Helpers::SamplesToBytes(vector<T>& samplesIn) {
        const size_t byteSize = samplesIn.size() * sizeof(T);
        vector<uint8_t> bytesOut(byteSize);
        std::memcpy(bytesOut.data(), samplesIn.data(), byteSize);

        return bytesOut;
    }
    void ProcessBitmap(const std::string& asset,
                       FAssetMetadata<FBitmapProperties>& meta,
                       vector<uint8_t>& bytesOut) {
        FAssetProps* base      = &meta.Properties;
        const auto bitmapProps = dynamic_cast<FBitmapProperties*>(base);
        if (!bitmapProps) {
            std::cerr << "Provided template type doesn't match provided enum type "
                         "(i.e. EAssetType::BITMAP != FBitmapProperties)\n";
            return;
        }
        int x, y, nrChannels;
        const auto bitmapBytes =
          Helpers::LoadBitmapToVector(asset,
                                      x,
                                      y,
                                      nrChannels,
                                      bitmapProps->Format == EBitmapFormat::RGB ? 3 : 4);

        bytesOut = Compress(bitmapBytes);
    }
    void ProcessAudio(const std::string& asset,
                      FAssetMetadata<FAudioProperties>& meta,
                      vector<uint8_t>& bytesOut) {
        FAssetProps* base     = &meta.Properties;
        const auto audioProps = dynamic_cast<FAudioProperties*>(base);
        if (!audioProps) {
            std::cerr << "Provided template type doesn't match provided enum type\n";
            return;
        }

        switch (audioProps->Encoding) {
            case EAudioFormat::FLOAT: {
                vector<float> samples;
                Helpers::LoadSamples<float>(asset, samples);
                const auto sampleBytes = Helpers::SamplesToBytes(samples);

                bytesOut = Compress(sampleBytes);
            } break;
            case EAudioFormat::INT: {
                vector<int32_t> samples;
                Helpers::LoadSamples<int32_t>(asset, samples);
                const auto sampleBytes = Helpers::SamplesToBytes(samples);

                bytesOut = Compress(sampleBytes);
            } break;
            case EAudioFormat::UINT: {
                vector<uint32_t> samples;
                Helpers::LoadSamples<uint32_t>(asset, samples);
                const auto sampleBytes = Helpers::SamplesToBytes(samples);

                bytesOut = Compress(sampleBytes);
            } break;
        }
    }

    void ProcessFont(const std::string& asset,
                     FAssetMetadata<FFontProperties>& meta,
                     vector<uint8_t>& bytesOut) {
        const auto assetBytes = IO::ReadAllBytes(asset).value();

        bytesOut = Compress(assetBytes);
    }

    template<typename PropType>
    void WritePak(const vector<std::string>& assets,
                  const fs::path& outDir,
                  const std::string& name,
                  FXPak& pakOut,
                  FXMeta<PropType>& metaOut) {
        for (const auto& asset : assets) {
            if (!fs::exists(asset)) {
                std::cerr << "[manifest.yaml] Asset: '" << asset << "' does not exist.\n";
                return;
            }

            const auto pos = asset.find_last_of('.');
            if (pos == std::string::npos || pos == 0) {
                break;
            }

            std::string metaFile = asset.substr(0, pos) + ".xasset";
            if (!fs::exists(metaFile)) {
                std::cerr << "[manifest.yaml] Meta file: '" << metaFile << "' does not exist.\n";
                return;
            }

            auto meta = XMeta::ReadAsset<PropType>(metaFile, true);
            metaOut.Assets.push_back(meta);

            FTableEntry tableEntry;
            strcpy(tableEntry.Path, asset.c_str());
            tableEntry.BlockOffset =
              HEADER_SIZE + (ENTRY_SIZE * assets.size()) + pakOut.DataBlocks.size();

            vector<uint8_t> compressedData;
            if constexpr (std::is_same_v<PropType, FBitmapProperties>) {
                ProcessBitmap(asset, meta, compressedData);
            } else if constexpr (std::is_same_v<PropType, FAudioProperties>) {
                ProcessAudio(asset, meta, compressedData);
            } else if constexpr (std::is_same_v<PropType, FFontProperties>) {
                ProcessFont(asset, meta, compressedData);
            }

            pakOut.DataBlocks.insert(pakOut.DataBlocks.end(),
                                     compressedData.begin(),
                                     compressedData.end());
            tableEntry.BlockSize = compressedData.size();

            pakOut.TableOfContents.push_back(tableEntry);
        }

        const uint32_t checksum = CRC32::CRC32(pakOut.DataBlocks.data(), pakOut.DataBlocks.size());
        pakOut.Header           = new FPakHeader(1, 0, checksum, pakOut.TableOfContents.size());

        const auto pakBytes = pakOut.Serialize();
        const auto pakName  = fmt::format("{}.xpak", name);
        IO::WriteAllBytes(outDir / pakName, pakBytes);

        auto metaBytes           = metaOut.Serialize();
        auto compressedMetaBytes = Compress(metaBytes);
        const auto metaName      = fmt::format("{}.xmeta", name);
        IO::WriteAllBytes(outDir / metaName, compressedMetaBytes);
    }

    void Pack(const YAML::Node& manifest, const fs::path& outDir) {
        CRC32::GenerateTable();

        const auto bitmaps = Helpers::CopyNodeToVector(manifest, "bitmaps");
        const auto audio   = Helpers::CopyNodeToVector(manifest, "audio");
        const auto fonts   = Helpers::CopyNodeToVector(manifest, "fonts");

        auto numAssets = bitmaps.size() + audio.size() + fonts.size();
        std::cout << "Packing " << numAssets << " assets...\n\n";

        if (!exists(outDir)) {
            create_directory(outDir);
        }

        // BITMAPS
        {
            std::cout << "Packing bitmaps\n";
            FXMeta<FBitmapProperties> bitmapsMeta;
            FXPak bitmapsPak;
            WritePak(bitmaps, outDir, "xen_bitmaps", bitmapsPak, bitmapsMeta);
            std::cout << "Done.\n\n";
        }

        // AUDIO
        {
            std::cout << "Packing audio\n";
            FXMeta<FAudioProperties> audioMeta;
            FXPak audioPak;
            WritePak(audio, outDir, "xen_audio", audioPak, audioMeta);
            std::cout << "Done.\n\n";
        }

        // FONTS
        {
            std::cout << "Packing fonts\n";
            FXMeta<FFontProperties> fontsMeta;
            FXPak fontsPak;
            WritePak(fonts, outDir, "xen_fonts", fontsPak, fontsMeta);
            std::cout << "Done.\n\n";
        }

        std::cout << "Packing finished. Files have been written to path: " << outDir << "\n";
    }

    std::vector<FXAsset> Unpack(const fs::path& pakFile) {
        vector<FXAsset> assets;

        // Find all xmeta and xpak files in dataDir
        if (!fs::exists(pakFile)) {
            std::cerr << "Pak file '" << pakFile.string() << "' does not exist.\n";
            return {};
        }

        // Iterate over each one, reading in the table of contents
        auto headerBytes      = IO::ReadBlock(pakFile, 0, HEADER_SIZE).value();
        const auto header     = FPakHeader::Deserialize(headerBytes);
        const auto numEntries = header->NumEntries;

        // Validate checksum
        const auto headerAndTableSize = HEADER_SIZE + (ENTRY_SIZE * numEntries);
        const auto blockSize          = fs::file_size(pakFile) - headerAndTableSize;
        auto blockBytes               = IO::ReadBlock(pakFile, headerAndTableSize, blockSize);

        const auto result = IO::ReadBlock(pakFile, HEADER_SIZE, ENTRY_SIZE * numEntries);
        if (!result.has_value()) {
            std::cerr << "Error reading ToC block from pak file: " << pakFile << "\n"
                      << "Block size: " << ENTRY_SIZE * numEntries << " bytes\n";
            return {};
        }
        const auto& tocBytes = result.value();
        for (int i = 0; i < numEntries; ++i) {
            FXAsset asset;
            vector<uint8_t> entryBytes(ENTRY_SIZE);
            std::memcpy(entryBytes.data(), tocBytes.data() + (i * ENTRY_SIZE), ENTRY_SIZE);
            const auto entry = FTableEntry::Deserialize(entryBytes);

            // Store each ToC entry and the pak file it was found in in FXAsset
            asset.Entry   = entry;
            asset.PakFile = pakFile.string();

            // Push asset to returning vector
            assets.push_back(asset);
        }

        return assets;
    }

    std::vector<FXAsset> UnpackAll(const fs::path& dataDir) {
        vector<FXAsset> assets;

        // Find all xmeta and xpak files in dataDir
        if (!fs::exists(dataDir)) {
            std::cerr << "Data directory '" << dataDir.string() << "' does not exist.\n";
            return {};
        }

        // Iterate over each one, reading in the table of contents
        for (auto fileMap = Helpers::GetPakAndMetaFiles(dataDir);
             const auto& pakFile : fileMap | std::views::keys) {
            auto headerBytes      = IO::ReadBlock(pakFile, 0, HEADER_SIZE).value();
            const auto header     = FPakHeader::Deserialize(headerBytes);
            const auto numEntries = header->NumEntries;
            // TODO: Validate checksum

            auto result = IO::ReadBlock(pakFile, HEADER_SIZE, ENTRY_SIZE * numEntries);
            if (!result.has_value()) {
                std::cerr << "Error reading ToC block from pak file: " << pakFile << "\n"
                          << "Block size: " << ENTRY_SIZE * numEntries << " bytes\n";
                break;
            }
            auto tocBytes = result.value();
            for (int i = 0; i < numEntries; ++i) {
                FXAsset asset;
                vector<uint8_t> entryBytes(ENTRY_SIZE);
                std::memcpy(entryBytes.data(), tocBytes.data() + (i * ENTRY_SIZE), ENTRY_SIZE);
                const auto entry = FTableEntry::Deserialize(entryBytes);

                // Store each ToC entry and the pak file it was found in in FXAsset
                asset.Entry   = entry;
                asset.PakFile = pakFile.string();

                // Push asset to returning vector
                assets.push_back(asset);
            }
        }

        return assets;
    }

    std::vector<unsigned char> FetchAsset(const std::string& path, const vector<FXAsset>& assets) {
        const FXAsset* foundAsset = nullptr;

        // Find asset entry in assets vector
        for (auto& asset : assets) {
            if (asset.Entry.Path == path) {
                foundAsset = &asset;
            }
        }

        if (!foundAsset) {
            std::cerr << "No asset with provided path was found.\n";
            return {};
        }

        // Read data block from associated pak file
        const auto pakFile = foundAsset->PakFile;
        if (!std::filesystem::exists(pakFile)) {
            std::cerr << "Pak file associated with asset does not exist on disk\n";
            return {};
        }

        const auto dataBytes =
          IO::ReadBlock(pakFile, foundAsset->Entry.BlockOffset, foundAsset->Entry.BlockSize);

        if (!dataBytes.has_value()) {
            std::cerr << "Could not read data from pak file. Block offset or block size may be "
                         "incorrect.\n";
            return {};
        }

        // Decompress data block
        auto decompressedData = Decompress(dataBytes.value());

        // Return data block bytes
        return decompressedData;
    }

    std::vector<unsigned char> FetchAssetFromDisk(const std::string& path) {
        // Find asset at path
        if (!fs::exists(path)) {
            std::cerr << "No asset with provided path was found.\n";
            return {};
        }

        // Read in to byte vector
        auto assetBytes = IO::ReadAllBytes(path);
        if (!assetBytes.has_value()) {
            std::cerr << "Could not read asset file data.\n";
            return {};
        }

        // Return byte vector
        return assetBytes.value();
    }
}  // namespace XPak

namespace XMeta {
    std::string GenerateGUID() {
        std::stringstream ss;
        const auto guid = xg::newGuid();
        ss << guid;
        return ss.str();
    }

    FAssetProps::~FAssetProps() = default;
    void FBitmapProperties::ReadProperties(const YAML::Node& node) {
        if (!node["BitmapProperties"].IsDefined()) {
            std::cerr << "BitmapProperties not defined in asset file.\n";
            exit(-1);
        }

        auto propsNode    = node["BitmapProperties"];
        Width             = propsNode["width"].as<uint32_t>();
        Height            = propsNode["height"].as<uint32_t>();
        const auto format = propsNode["format"].as<std::string>();
        if (format == "RGB") {
            Format = EBitmapFormat::RGB;
        }
        if (format == "RGBA") {
            Format = EBitmapFormat::RGBA;
        }
        if (format == "ARGB") {
            Format = EBitmapFormat::ARGB;
        }
    }

    constexpr size_t FBitmapProperties::GetSize() {
        return sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint8_t);
    }

    void FAudioProperties::ReadProperties(const YAML::Node& node) {
        if (!node["AudioProperties"].IsDefined()) {
            std::cerr << "AudioProperties not defined in asset file.\n";
            exit(-1);
        }

        auto propsNode      = node["AudioProperties"];
        Channels            = propsNode["channels"].as<uint32_t>();
        BitSize             = propsNode["bitsize"].as<uint32_t>();
        const auto encoding = propsNode["encoding"].as<std::string>();
        if (encoding == "Float") {
            Encoding = EAudioFormat::FLOAT;
        }
        if (encoding == "Int") {
            Encoding = EAudioFormat::INT;
        }
        if (encoding == "UInt") {
            Encoding = EAudioFormat::UINT;
        }
    }

    constexpr size_t FAudioProperties::GetSize() {
        return sizeof(uint32_t) + sizeof(uint8_t) + sizeof(uint32_t);
    }

    void FFontProperties::ReadProperties(const YAML::Node& node) {
        if (!node["FontProperties"].IsDefined()) {
            std::cerr << "FontProperties not defined in asset file.\n";
            exit(-1);
        }

        auto propsNode  = node["FontProperties"];
        const auto type = propsNode["type"].as<std::string>();
        if (type == "TrueType") {
            Type = EFontType::TRUE_TYPE;
        }
        if (type == "OpenType") {
            Type = EFontType::OPEN_TYPE;
        }
        const auto family = propsNode["family"].as<std::string>();
        if (family == "Monospace") {
            Family = EFontFamily::MONOSPACE;
        }
        if (family == "Serif") {
            Family = EFontFamily::SERIF;
        }
        if (family == "SansSerif") {
            Family = EFontFamily::SANS_SERIF;
        }
        if (family == "Display") {
            Family = EFontFamily::DISPLAY;
        }
    }

    constexpr size_t FFontProperties::GetSize() {
        return sizeof(uint8_t) + sizeof(uint8_t);
    }

    template<typename PropType>
    FAssetMetadata<PropType>::FAssetMetadata() {
        GUID = GenerateGUID();
    }

    template<typename PropType>
    FAssetMetadata<PropType> ReadAsset(const std::string& data, const bool isFile) {
        static_assert(std::is_base_of_v<FAssetProps, PropType>, "Incorrect PropType provided");

        FAssetMetadata<PropType> asset;

        YAML::Node assetFile;
        if (isFile)
            assetFile = YAML::LoadFile(data);
        else
            assetFile = YAML::Load(data);

        asset.GUID    = assetFile["guid"].as<std::string>();
        asset.Version = assetFile["version"].as<uint32_t>();
        // We know the `ReadProperties` method exists because of the static assert above.
        asset.Properties.ReadProperties(assetFile);
        asset.YamlNode = assetFile;

        return asset;
    }

    template<typename PropType>
    FXMeta<PropType>::FXMeta() {
        static_assert(std::is_base_of_v<FAssetProps, PropType>,
                      "Invalid PropType provided. Must be child of FAssetProps");
    }

    template<typename PropType>
    std::vector<unsigned char> FXMeta<PropType>::Serialize() {
        const size_t totalSize = (Assets.size() * META_SIZE);
        vector<uint8_t> bytes(totalSize, '\0');

        size_t i = 0;
        for (auto& asset : Assets) {
            std::stringstream yaml;
            yaml << asset.YamlNode;
            std::string yamlStr = yaml.str();

            if (yamlStr.length() > META_SIZE) {
                std::cerr << "Metadata for asset exceeds maximum allowed length.\n";
                exit(-1);
            }

            auto stringBytes =
              vector<uint8_t>(reinterpret_cast<const uint8_t*>(yamlStr.data()),
                              reinterpret_cast<const uint8_t*>(yamlStr.data() + yamlStr.size()));

            std::memcpy(bytes.data() + (i * META_SIZE), stringBytes.data(), stringBytes.size());
            ++i;
        }

        return bytes;
    }

    template<typename PropType>
    FXMeta<PropType> FXMeta<PropType>::Deserialize(const vector<uint8_t>& bytes) {
        if (bytes.size() % META_SIZE != 0) {
            std::cerr << "Invalid byte count for metadata, possible corruption.\n";
            exit(-1);
        }

        FXMeta<PropType> meta;

        const auto numEntries = bytes.size() / META_SIZE;
        for (int i = 0; i < numEntries; ++i) {
            vector<uint8_t> entryBytes(META_SIZE, '\0');
            std::memcpy(entryBytes.data(), bytes.data() + (i * META_SIZE), META_SIZE);

            auto lastNonNull = std::find_if(entryBytes.rbegin(),
                                            entryBytes.rend(),
                                            [](uint8_t byte) { return byte != 0; });

            if (lastNonNull != entryBytes.rend()) {
                entryBytes.erase(lastNonNull.base(), entryBytes.end());
            }

            auto asset = ReadAsset<PropType>(std::string(entryBytes.begin(), entryBytes.end()));
            meta.Assets.push_back(asset);
        }

        return meta;
    }

    template<typename PropType>
    void FXMeta<PropType>::Print() {
        for (auto& asset : Assets) {
            std::stringstream ss;
            ss << asset.YamlNode;
            std::cout << ss.str() << "\n\n";
        }

        std::cout << "File information:\n";
        std::cout << "Total Assets      => " << Assets.size() << "\n";
        std::cout << "Uncompressed Size => " << Assets.size() * META_SIZE << " bytes\n";
    }

    template<typename PropType>
    void Print(const std::string& path) {
        assert(std::filesystem::exists(path));
        const auto fileBytes   = IO::ReadAllBytes(path).value();
        auto decompressedBytes = XPak::Decompress(fileBytes);
        FXMeta<PropType> meta  = FXMeta<PropType>::Deserialize(decompressedBytes);
        meta.Print();
        std::cout << "Compressed Size   => " << fileBytes.size() << " bytes\n";
        std::cout << "Compression Ratio => "
                  << 100.f - (static_cast<float>(fileBytes.size()) /
                              static_cast<float>(decompressedBytes.size())) *
                               100.f
                  << "%\n";
    }
}  // namespace XMeta