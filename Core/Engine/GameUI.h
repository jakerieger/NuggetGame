// Author: Jake Rieger
// Created: 3/28/2024.
//

#pragma once

#include "STL.h"

namespace UI {
    void Initialize();
    void Begin();
    void Draw();
    void End();
    void Shutdown();

    void LoadDocument(const std::string& source);
}  // namespace UI