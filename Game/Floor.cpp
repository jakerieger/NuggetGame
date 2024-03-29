// Author: Jake Rieger
// Created: 3/26/2024.
//

#include "Floor.h"

Floor::Floor(const std::string& name) : IGameObject(name) {
    const auto floorSprite =
      Resources::GetResource<Packer::Schemas::Sprite>(Resources::ResourceType::Sprite, "floor.png");

    static constexpr f32 TILE_DENOM      = 32.f;
    static constexpr f32 TILE_OFFSET     = TILE_DENOM;
    static constexpr f32 TILE_SIZE       = TILE_DENOM / 2.f;
    static constexpr u32 NUM_FLOOR_TILES = 640 / (TILE_SIZE * 2.f);
    static constexpr u32 HEIGHT          = 1;

    std::vector<glm::vec2> tiles(NUM_FLOOR_TILES);

    for (auto i = 0; i < NUM_FLOOR_TILES; ++i) {
        for (auto k = 0; k < HEIGHT; ++k) {
            const auto _i = static_cast<f32>(i);
            const auto _k = static_cast<f32>(k);

            auto x = TILE_OFFSET * _i + TILE_SIZE;
            auto y = (_k * TILE_SIZE * 2) + TILE_SIZE;

            tiles.emplace_back(x, y);
        }
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
