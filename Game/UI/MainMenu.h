// Author: Jake Rieger
// Created: 4/12/2024.
//

#pragma once

#include "Components/GUI.h"
#include "Interfaces/UIDocument.h"
#include "Interfaces/GameObject.h"

class MainMenuListener final : public IUIListener {
public:
    void ProcessEvent(Rml::Event& event) override;
};

class MainMenuDocument : public IUIDocument {
public:
    MainMenuDocument();
};

class MainMenu final : public IGameObject {
public:
    explicit MainMenu(const std::string& name) : IGameObject(name) {
        m_GUI = new AGUI(new MainMenuDocument);
    }

private:
    AGUI* m_GUI = nullptr;
};