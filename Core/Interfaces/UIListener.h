// Author: Jake Rieger
// Created: 4/16/2024.
//

#pragma once

#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/EventListener.h>

class IUIListener : public Rml::EventListener {
public:
    void RegisterButton(Rml::Element* element) {
        element->AddEventListener(Rml::EventId::Mousedown, this);
    }

    void ProcessEvent(Rml::Event& event) override {}
};