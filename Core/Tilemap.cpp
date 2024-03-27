// Author: Jake Rieger
// Created: 3/26/2024.
//

#include "Tilemap.h"
#include "Utilities.inl"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

static void UpdateModelMatrices(const std::vector<glm::vec2>& tiles,
                                float tileSize,
                                std::vector<glm::mat4>& outMatrices) {
    for (const auto& tile : tiles) {
        auto transform = glm::mat4(1.f);
        transform      = glm::translate(transform, {tile.x, tile.y, 0.f});
        transform      = glm::scale(transform, {tileSize, tileSize, 1.f});
        outMatrices.push_back(transform);
    }
}

ATilemap::ATilemap(const Packer::Schemas::Sprite& sprite,
                   const std::vector<glm::vec2>& tiles,
                   const float tileSize)
    : m_TileSize(tileSize), m_Tiles(tiles) {
    m_SpriteBatch = new ASpriteBatch();
    UpdateModelMatrices(tiles, tileSize, m_ModelMatrices);

    m_Sprite =
      Utilities::LoadTextureFromData(sprite.data, sprite.width, sprite.height, sprite.channels);
    assert(m_Sprite != 0);
}

void ATilemap::Start(FSceneContext& sceneContext) {
    IComponent::Start(sceneContext);
    m_SpriteBatch->Init(m_ModelMatrices);
}

void ATilemap::Update(float deltaTime, FSceneContext& sceneContext) {
    IComponent::Update(deltaTime, sceneContext);
    // UpdateModelMatrices(m_Tiles, m_TileSize, m_ModelMatrices);
    m_SpriteBatch->UpdateInstances(m_ModelMatrices);
}

void ATilemap::Draw(FSceneContext& sceneContext) const {
    const auto projection = glm::ortho(0.f, 640.f, 0.f, 360.f);
    m_SpriteBatch->Draw(m_Sprite, projection, m_ModelMatrices.size());
}

void ATilemap::Destroyed(FSceneContext& sceneContext) {
    IComponent::Destroyed(sceneContext);
    m_SpriteBatch->Cleanup();
    delete m_SpriteBatch;
}