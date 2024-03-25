// Author: Jake Rieger
// Created: 3/25/2024.
//

#pragma once

#include "Transform.h"
#include "Interfaces/Component.h"
#include "STL.h"
#include "Sprite.h"

class AShader;

class ASpriteRenderer final : public IComponent {
public:
    explicit ASpriteRenderer(const char* resource);

    void Start(FSceneContext& sceneContext) override;
    void Update(float deltaTime, FSceneContext& sceneContext) override;
    void Draw(FSceneContext& sceneContext, const ATransform* transform) const;
    void Destroyed(FSceneContext& sceneContext) override;

private:
    std::unique_ptr<AQuad> m_Quad;
    u32 m_SpriteId = 0;
};
