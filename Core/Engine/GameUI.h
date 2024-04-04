// Author: Jake Rieger
// Created: 3/28/2024.
//

#pragma once

#include "STL.h"

#include <RmlUi/Core/ElementDocument.h>

namespace UI {
    void Initialize();
    void Begin();
    void Draw();
    void End();
    void Shutdown();

    Rml::ElementDocument* CreateDocument(const std::string& source);
    void CloseDocument(Rml::ElementDocument* document);
}  // namespace UI