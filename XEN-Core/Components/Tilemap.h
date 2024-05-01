// Author: Jake Rieger
// Created: 3/26/2024.
//

#pragma once

#include "STL.h"
#include "SpriteBatch.h"
#include "Interfaces/Component.h"

#include <glm/vec2.hpp>

class ATilemap final : public IComponent {
public:
    explicit ATilemap(void* sprite, const std::vector<glm::vec2>& tiles, float tileSize);

    void Start(FSceneContext& sceneContext) override;
    void Update(float deltaTime, FSceneContext& sceneContext) override;
    void Draw(FSceneContext& sceneContext) const;
    void Destroyed(FSceneContext& sceneContext) override;

private:
    u32 m_Sprite = 0;
    float m_TileSize;
    std::vector<glm::vec2> m_Tiles;
    std::vector<glm::mat4> m_ModelMatrices;
    ASpriteBatch* m_SpriteBatch = nullptr;
};
