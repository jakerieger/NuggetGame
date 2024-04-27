// Author: Jake Rieger
// Created: 4/26/2024.
//

#pragma once

#include <cpp-base64/base64.h>
#include <rapidjson/rapidjson.h>

enum class EAssetType {
    Sprite,
    Audio,
    Level,
    Font,
    Movie,
};

class AssetDescriptor {};
