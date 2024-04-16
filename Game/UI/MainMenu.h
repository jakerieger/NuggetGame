// Author: Jake Rieger
// Created: 4/12/2024.
//

#pragma once

#include "Interfaces/UIDocument.h"

#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/EventListener.h>
#include <RmlUi/Core/StyleSheet.h>

using Rml::Element;
using Rml::ElementDocument;
using Rml::EventListener;

class MainMenuListener final : public IUIListener {
public:
    void ProcessEvent(Rml::Event& event) override {
        if (event.GetId() == Rml::EventId::Mousedown) {
            auto elementId = event.GetCurrentElement()->GetId();
            if (strcmp(elementId.c_str(), "btn-play") == 0) {
                // Load level select scene
                Application::GetCurrentApp()->LoadScene("LevelSelect");
            }
            if (strcmp(elementId.c_str(), "btn-settings") == 0) {
                // Load settings screen
                Application::GetCurrentApp()->LoadScene("Settings");
            }
            if (strcmp(elementId.c_str(), "btn-quit") == 0) {
                Graphics::MarkWindowForClose();
            }
        }
    }
};

class MainMenu : public IUIDocument {
public:
    MainMenu() {
        m_Listener = new MainMenuListener;
        assert(m_Listener);

        const auto documentPath = Resources::GetRoot() / "Interface" / "MainMenu.html";
        m_Document              = UI::GetContext()->LoadDocument(documentPath.string());
        assert(m_Document != nullptr);

        m_Title          = m_Document->GetElementById("title");
        m_PlayButton     = m_Document->GetElementById("btn-play");
        m_SettingsButton = m_Document->GetElementById("btn-settings");
        m_QuitButton     = m_Document->GetElementById("btn-quit");
        assert(m_Title != nullptr);
        assert(m_PlayButton != nullptr);
        assert(m_SettingsButton != nullptr);
        assert(m_QuitButton != nullptr);

        this->m_Listener->RegisterButton(m_PlayButton);
        this->m_Listener->RegisterButton(m_SettingsButton);
        this->m_Listener->RegisterButton(m_QuitButton);
    }

private:
    Element* m_Title          = nullptr;
    Element* m_PlayButton     = nullptr;
    Element* m_SettingsButton = nullptr;
    Element* m_QuitButton     = nullptr;
};