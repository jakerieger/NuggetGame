// Author: Jake Rieger
// Created: 5/15/24.
//

#include <AssetDescriptor.h>
#include <catch2/catch_test_macros.hpp>

using namespace AssetTool;

const ByteArray sourceData = {'A', 'B', 'C', 'D', 'E'};
constexpr u32 version      = 1;

TEST_CASE("Descriptor.Serialize", "[XEN-AssetTool]") {
    // ========================================================================================== //
    SpriteDescriptor spriteDescriptor;
    spriteDescriptor.m_Name                                       = "TestSprite";
    spriteDescriptor.m_Type                                       = AssetType::Sprite;
    spriteDescriptor.m_Version                                    = version;
    spriteDescriptor.m_SrcData                                    = sourceData;
    spriteDescriptor.GetProperties<SpriteProperties>()->m_Width   = 32;
    spriteDescriptor.GetProperties<SpriteProperties>()->m_Height  = 32;
    spriteDescriptor.GetProperties<SpriteProperties>()->m_IsAlpha = true;

    const ByteArray spriteBytes = spriteDescriptor.Serialize();
    const ByteArray spritePropsBytes =
      spriteDescriptor.GetProperties<SpriteProperties>()->Serialize();

    REQUIRE(spriteBytes.size() == spriteDescriptor.GetSize());
    REQUIRE(spritePropsBytes.size() == SpriteProperties::Size());
    // ========================================================================================== //
    FontDescriptor fontDescriptor;
    fontDescriptor.m_Name                                         = "TestFont";
    fontDescriptor.m_Type                                         = AssetType::Font;
    fontDescriptor.m_Version                                      = version;
    fontDescriptor.m_SrcData                                      = sourceData;
    fontDescriptor.GetProperties<FontProperties>()->m_DefaultSize = 24;

    const ByteArray fontBytes      = fontDescriptor.Serialize();
    const ByteArray fontPropsBytes = fontDescriptor.GetProperties<FontProperties>()->Serialize();

    REQUIRE(fontBytes.size() == fontDescriptor.GetSize());
    REQUIRE(fontPropsBytes.size() == FontProperties::Size());
    // ========================================================================================== //
    AudioDescriptor audioDescriptor;
    audioDescriptor.m_Name                                          = "TestAudio";
    audioDescriptor.m_Type                                          = AssetType::Audio;
    audioDescriptor.m_Version                                       = version;
    audioDescriptor.m_SrcData                                       = sourceData;
    audioDescriptor.GetProperties<AudioProperties>()->m_SampleCount = 500000;
    audioDescriptor.GetProperties<AudioProperties>()->m_SampleRate  = 48000;
    audioDescriptor.GetProperties<AudioProperties>()->m_Channels    = 2;

    const ByteArray audioBytes      = audioDescriptor.Serialize();
    const ByteArray audioPropsBytes = audioDescriptor.GetProperties<AudioProperties>()->Serialize();

    REQUIRE(audioBytes.size() == audioDescriptor.GetSize());
    REQUIRE(audioPropsBytes.size() == AudioProperties::Size());
    // ========================================================================================== //
    LevelDescriptor levelDescriptor;
    levelDescriptor.m_Name                                                = "TestLevel";
    levelDescriptor.m_Type                                                = AssetType::Level;
    levelDescriptor.m_Version                                             = version;
    levelDescriptor.m_SrcData                                             = sourceData;
    levelDescriptor.GetProperties<LevelProperties>()->m_Rows              = 100;
    levelDescriptor.GetProperties<LevelProperties>()->m_Columns           = 100;
    levelDescriptor.GetProperties<LevelProperties>()->m_PlayerStart       = {0.f, 0.f};
    levelDescriptor.GetProperties<LevelProperties>()->m_ObjectivePosition = {20.f, 0.f};

    const ByteArray levelBytes      = levelDescriptor.Serialize();
    const ByteArray levelPropsBytes = levelDescriptor.GetProperties<LevelProperties>()->Serialize();

    REQUIRE(levelBytes.size() == levelDescriptor.GetSize());
    REQUIRE(levelPropsBytes.size() == LevelProperties::Size());
}