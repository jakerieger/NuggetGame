// Author: Jake Rieger
// Created: 4/16/2024.
//

#pragma once

#include "Interfaces/GameObject.h"
#include "Interfaces/UIDocument.h"

class SettingsMenuListener final : public IUIListener {
public:
    void ProcessEvent(Rml::Event& event) override;
};

class SettingsMenuDocument final : public IUIDocument {
public:
    SettingsMenuDocument();
};

class SettingsMenu final : public IGameObject {
public:
    explicit SettingsMenu(const std::string& name) : IGameObject(name) {}
    void Initialize() override;
    void Start(FSceneContext& sceneContext) override;
    void Destroyed(FSceneContext& sceneContext) override;

private:
    IUIDocument* m_Document = nullptr;
};
