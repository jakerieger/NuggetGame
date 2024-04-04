// Author: Jake Rieger
// Created: 4/2/2024.
//

#pragma once

#include "STL.h"

namespace Audio {
    void Initialize();
    void PlayOneShot(const std::string& filename, f32 gain = 1.f);
    void PlayLoop(const std::string& filename, f32 gain = 1.f);
    void Shutdown();
}  // namespace Audio
