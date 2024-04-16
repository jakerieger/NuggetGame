// Author: Jake Rieger
// Created: 4/16/2024.
//

#include "GUI.h"
#include "Interfaces/UIDocument.h"

AGUI::AGUI(IUIDocument* document) {
    m_Document = document;
    m_Document->Show();
}