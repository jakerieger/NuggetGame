// Author: Jake Rieger
// Created: 4/16/2024.
//

#pragma once

#include "Interfaces/Component.h"

class IUIDocument;

class AGUI final : public IComponent {
public:
    explicit AGUI(IUIDocument* document);

protected:
    IUIDocument* m_Document;
};
