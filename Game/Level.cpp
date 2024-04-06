// Author: Jake Rieger
// Created: 4/4/2024.
//

#include "Level.h"
#include "Logger.h"
#include "Engine/PhysicsContext.h"

static constexpr f32 k_TileSize = 2.f;

Level::Level(const std::string& name, const FLevel& levelData) : IGameObject(name) {
    using Packer::Schemas::Sprite;
    assert(levelData.Tiles.size() == levelData.Columns * levelData.Rows);

    m_SpriteMap[1] = &Resources::GetResource<Sprite>(Resources::ResourceType::Sprite, "floor.png");
    m_SpriteMap[2] =
      &Resources::GetResource<Sprite>(Resources::ResourceType::Sprite, "ramp_left.png");
    m_SpriteMap[3] =
      &Resources::GetResource<Sprite>(Resources::ResourceType::Sprite, "ramp_right.png");
    m_SpriteMap[4] =
      &Resources::GetResource<Sprite>(Resources::ResourceType::Sprite, "button_unpressed.png");
    m_SpriteMap[5] =
      &Resources::GetResource<Sprite>(Resources::ResourceType::Sprite, "button_pressed.png");

    for (auto i = 0; i < levelData.Rows; ++i) {
        std::vector<i32> row;
        for (auto j = 0; j < levelData.Columns; ++j) {
            row.push_back(levelData.Tiles.at(i * levelData.Columns + j));
        }
        m_TileIds.push_back(row);
    }

    // Invert tile id map to match coordinate system in game
    std::ranges::reverse(m_TileIds);

    // Generate x,y positions for each tile
    auto y = 0;
    for (const auto& row : m_TileIds) {
        y++;
        auto x = 0;
        for (auto id : row) {
            x++;
            auto xPos = static_cast<f32>(x) * k_TileSize * 2.f - 2.f;
            auto yPos = static_cast<f32>(y) * k_TileSize * 2.f - 2.f;
            // 0 is the empty tile, we can ignore it to save memory
            if (id != 0) {
                m_TilePositions[id].emplace_back(xPos, yPos);

                // Create the colliders
                b2BodyDef bodyDef;
                bodyDef.position.Set(xPos, yPos + 0.5f);
                const auto body = Physics::GetWorld()->CreateBody(&bodyDef);
                b2PolygonShape bodyShape;
                bodyShape.SetAsBox(2.f, 2.f);
                body->CreateFixture(&bodyShape, 0.f);
            }
        }
    }

    // Create a tilemap for each tile
    for (auto& [id, positions] : m_TilePositions) {
        auto tilemap = new ATilemap(*m_SpriteMap[id], positions, k_TileSize);
        m_Tilemaps.push_back(tilemap);
    }
}

void Level::Start(FSceneContext& sceneContext) {
    IGameObject::Start(sceneContext);
    for (const auto tile : m_Tilemaps) {
        tile->Start(sceneContext);
    }
}

void Level::Update(const float deltaTime, FSceneContext& sceneContext) {
    IGameObject::Update(deltaTime, sceneContext);
    for (const auto tile : m_Tilemaps) {
        tile->Update(deltaTime, sceneContext);
    }
}

void Level::Draw(FSceneContext& sceneContext) {
    for (const auto tile : m_Tilemaps) {
        tile->Draw(sceneContext);
    }
}

void Level::Destroyed(FSceneContext& sceneContext) {
    IGameObject::Destroyed(sceneContext);

    for (const auto tile : m_Tilemaps) {
        tile->Destroyed(sceneContext);
        delete tile;
    }
}