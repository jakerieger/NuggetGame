// Author: Jake Rieger
// Created: 4/12/2024.
//

#pragma once

#include "Interfaces/GameObject.h"
#include "Interfaces/UIDocument.h"

class MainMenuListener final : public IUIListener {
public:
    void ProcessEvent(Rml::Event& event) override;
};

class MainMenuDocument final : public IUIDocument {
public:
    MainMenuDocument();
};

class MainMenu final : public IGameObject {
public:
    explicit MainMenu(const std::string& name) : IGameObject(name) {}
    void Initialize() override;

    void Start(FSceneContext& sceneContext) override;
    void Destroyed(FSceneContext& sceneContext) override;

private:
    IUIDocument* m_Document = nullptr;
};