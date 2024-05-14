// Author: Jake Rieger
// Created: 5/14/24.
//

#include <AssetMetadata.h>
#include <AssetManifest.h>
#include <AssetDescriptor.h>
#include <AssetPak.h>

#include <catch2/catch_test_macros.hpp>

using namespace AssetTool;

TEST_CASE("Metadata", "[AssetTool]") {
    constexpr size_t compSize = 499999;
    constexpr size_t origSize = 999999;
    constexpr u32 manifestCt  = 4;
    constexpr char checksum[] = "40aa1b203c9d8ee150b21c3c7cda8261492e5420c5f2b9f7380700e094c303b4"
                                "8e62f319c1da0e32eb40d113c5f1749cc61aeb499167890ab82f2cc9bb706971";

    AssetMetadata metadata;
    metadata.m_Checksum       = checksum;
    metadata.m_CompressedSize = compSize;
    metadata.m_OriginalSize   = origSize;
    metadata.m_ManifestCount  = manifestCt;

    auto bytes = metadata.Serialize();
    REQUIRE(bytes.size() == 148);

    auto parsedMetadata = AssetMetadata::Deserialize(bytes);
    REQUIRE(parsedMetadata.m_Checksum == std::string(checksum));
    REQUIRE(parsedMetadata.m_CompressedSize == compSize);
    REQUIRE(parsedMetadata.m_OriginalSize == origSize);
    REQUIRE(parsedMetadata.m_ManifestCount == manifestCt);
}

TEST_CASE("Descriptor", "[AssetTool]") {
    REQUIRE(true);
}

TEST_CASE("Manifest", "[AssetTool]") {
    REQUIRE(true);
}

TEST_CASE("Packer", "[AssetTool]") {
    REQUIRE(true);
}

TEST_CASE("Unpacker", "[AssetTool]") {
    REQUIRE(true);
}