// Author: Jake Rieger
// Created: 4/16/2024.
//

#include "MainMenu.h"

#include <Engine/GameApp.h>
#include <Engine/GameUI.h>
#include <Engine/GraphicsContext.h>
#include <Engine/Resources.h>
#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/Context.h>

void MainMenuListener::ProcessEvent(Rml::Event& event) {
    if (event.GetId() == Rml::EventId::Mousedown) {
        const auto elementId = event.GetCurrentElement()->GetId();
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

MainMenuDocument::MainMenuDocument() {
    m_Listener = new MainMenuListener;
    assert(m_Listener);

    const auto documentPath = Resources::GetRoot() / "Interface" / "MainMenu.html";
    m_Document              = UI::GetContext()->LoadDocument(documentPath.string());
    assert(m_Document != nullptr);

    m_ElementMap["title"]        = m_Document->GetElementById("title");
    m_ElementMap["btn-play"]     = m_Document->GetElementById("btn-play");
    m_ElementMap["btn-settings"] = m_Document->GetElementById("btn-settings");
    m_ElementMap["btn-quit"]     = m_Document->GetElementById("btn-quit");

    m_Listener->RegisterButton(m_ElementMap["btn-play"]);
    m_Listener->RegisterButton(m_ElementMap["btn-settings"]);
    m_Listener->RegisterButton(m_ElementMap["btn-quit"]);
}
