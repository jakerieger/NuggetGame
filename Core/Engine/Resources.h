#pragma once

#include "STL.h"

#define RES_SPRITE "sprites"
#define RES_ROOT ""

namespace Resources {
    void SetCwd(const char* exePath);
    std::string GetResource(const char* type, const char* name);
}  // namespace Resources