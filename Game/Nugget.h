// Author: Jake Rieger
// Created: 3/25/2024.
//

#pragma once

#include "Components/Rigidbody.h"
#include "Components/SpriteRenderer.h"
#include "Interfaces/Drawable.h"
#include "Interfaces/GameObject.h"

class Nugget final : public IGameObject,
                     public IDrawable {
public:
    explicit Nugget(const std::string& name);

    void Draw(FSceneContext& sceneContext) override;
    void Start(FSceneContext& sceneContext) override;
    void Update(float deltaTime, FSceneContext& sceneContext) override;
    void FixedUpdate(FSceneContext& sceneContext) override;
    void Destroyed(FSceneContext& sceneContext) override;

    void OnKeyDown(FKeyEvent& event) override;
    void OnKey(FKeyEvent& event) override;

    ASpriteRenderer* m_SpriteRenderer;
    ARigidbody* m_Rigidbody;
};
