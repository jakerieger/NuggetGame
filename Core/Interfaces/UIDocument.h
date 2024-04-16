// Author: Jake Rieger
// Created: 4/16/2024.
//

#pragma once

#include <RmlUi/Core/EventListener.h>

class IUIListener : public Rml::EventListener {
public:
    void RegisterButton(Rml::Element* element) {
        element->AddEventListener(Rml::EventId::Mousedown, this);
    }

    void ProcessEvent(Rml::Event& event) override {}
};

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