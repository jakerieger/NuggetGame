// Author: Jake Rieger
// Created: 4/16/2024.
//

#pragma once

#include "UIListener.h"
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/Element.h>

class IUIDocument {
public:
    void Show() const {
        m_Document->Show();
    }

    void Hide() const {
        m_Document->Hide();
    }

protected:
    Rml::ElementDocument* m_Document = nullptr;
    IUIListener* m_Listener          = nullptr;
    std::unordered_map<const char*, Rml::Element*> m_ElementMap;
};