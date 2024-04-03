// Author: Jake Rieger
// Created: 4/2/2024.
//

#include "Level.h"

#include <cassert>

TVec2i ALevel::ArrayTo2DVec(const u32 xTiles, const u32 yTiles, const std::vector<i32>& tiles) {
    assert(xTiles * yTiles == tiles.size());
    TVec2i grid;

    auto idx = 0;
    for (auto v : tiles) {
        if (idx % (xTiles - 1)) {
            
        }

        idx++;
    }

    return grid;
}