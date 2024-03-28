// Author: Jake Rieger
// Created: 3/26/2024.
//

#pragma once

#include "Components/Tilemap.h"
#include "Engine/Resources.h"
#include "Interfaces/Drawable.h"
#include "Interfaces/GameObject.h"

class Floor final : public IGameObject,
                    public IDrawable {
public:
    explicit Floor(const std::string& name);

    void Start(FSceneContext& sceneContext) override;
    void Update(float deltaTime, FSceneContext& sceneContext) override;
    void Draw(FSceneContext& sceneContext) override;
    void Destroyed(FSceneContext& sceneContext) override;

    ATilemap* m_Tilemap = nullptr;
};