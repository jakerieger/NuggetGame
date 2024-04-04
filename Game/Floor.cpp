// Author: Jake Rieger
// Created: 3/26/2024.
//

#include "Floor.h"

Floor::Floor(const std::string& name) : IGameObject(name) {
    const auto floorSprite =
      Resources::GetResource<Packer::Schemas::Sprite>(Resources::ResourceType::Sprite, "floor.png");

    static constexpr f32 TILE_SIZE = 2;
    std::vector<glm::vec2> tiles;

    for (auto i = -12; i < 12; i++) {
        tiles.emplace_back(i * 4, 2);
    }

    m_Tilemap = new ATilemap(floorSprite, tiles, TILE_SIZE);
    m_Tilemap->SetParent(this);

    RegisterComponent(m_Tilemap);
}

void Floor::Start(FSceneContext& sceneContext) {
    IGameObject::Start(sceneContext);
    m_Tilemap->Start(sceneContext);
}

void Floor::Update(const float deltaTime, FSceneContext& sceneContext) {
    IGameObject::Update(deltaTime, sceneContext);
    m_Tilemap->Update(deltaTime, sceneContext);
}

void Floor::Draw(FSceneContext& sceneContext) { m_Tilemap->Draw(sceneContext); }

void Floor::Destroyed(FSceneContext& sceneContext) {
    IGameObject::Destroyed(sceneContext);
    m_Tilemap->Destroyed(sceneContext);
}
