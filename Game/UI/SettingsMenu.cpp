// Author: Jake Rieger
// Created: 4/16/2024.
//

#include "SettingsMenu.h"
#include "Engine/GameApp.h"
#include "Engine/GraphicsContext.h"
#include "Engine/Resources.h"
#include "Engine/GameUI.h"

#include <RmlUi/Core/Context.h>

void SettingsMenuListener::ProcessEvent(Rml::Event& event) {
    if (event.GetId() == Rml::EventId::Mousedown) {
        const auto elementId = event.GetCurrentElement()->GetId();
        if (strcmp(elementId.c_str(), "btn-back") == 0) {
            event.StopImmediatePropagation();
            Application::GetCurrentApp()->LoadScene("MainMenu");
        }
    }
}

SettingsMenuDocument::SettingsMenuDocument() {
    m_Listener = new SettingsMenuListener;
    assert(m_Listener);

    const auto documentPath = Resources::GetRoot() / "Interface" / "SettingsMenu.html";
    m_Document              = UI::GetContext()->LoadDocument(documentPath.string());
    assert(m_Document != nullptr);

    m_Listener->RegisterButton(m_Document->GetElementById("btn-back"));
}

void SettingsMenu::Initialize() {
    m_Document = new SettingsMenuDocument;
}

void SettingsMenu::Start(FSceneContext& sceneContext) {
    IGameObject::Start(sceneContext);
    m_Document->Show();
}

void SettingsMenu::Destroyed(FSceneContext& sceneContext) {
    IGameObject::Destroyed(sceneContext);
    const auto document = m_Document->GetDocument();
    const auto listener = m_Document->GetListener();
    listener->UnregisterButton(document->GetElementById("btn-back"));
    m_Document->Hide();
    delete m_Document;
}
