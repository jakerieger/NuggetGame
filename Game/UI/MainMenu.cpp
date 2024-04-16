// Author: Jake Rieger
// Created: 4/16/2024.
//

#include "MainMenu.h"

#include "Engine/GameApp.h"
#include "Engine/GraphicsContext.h"
#include "Engine/Resources.h"
#include "Engine/GameUI.h"

#include <RmlUi/Core/Context.h>

void MainMenuListener::ProcessEvent(Rml::Event& event) {
    if (event.GetId() == Rml::EventId::Mousedown) {
        const auto elementId = event.GetCurrentElement()->GetId();
        if (strcmp(elementId.c_str(), "btn-play") == 0) {
            // Load level select scene
            Application::GetCurrentApp()->LoadScene("Test");
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

    m_Listener->RegisterButton(m_Document->GetElementById("btn-play"));
    m_Listener->RegisterButton(m_Document->GetElementById("btn-settings"));
    m_Listener->RegisterButton(m_Document->GetElementById("btn-quit"));
}

void MainMenu::Start(FSceneContext& sceneContext) {
    IGameObject::Start(sceneContext);
    m_Document->Show();
}

void MainMenu::Destroyed(FSceneContext& sceneContext) {
    IGameObject::Destroyed(sceneContext);
    m_Document->Hide();
}