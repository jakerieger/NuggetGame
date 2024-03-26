// Author: Jake Rieger
// Created: 3/25/2024.
//

#pragma once
#include "Packer.h"

#include <vector>

namespace Unpacker {
    void UnpackSprites(const std::filesystem::path& dataDir,
                       std::vector<Packer::Schemes::Sprite>* sprites);
}  // namespace Unpacker