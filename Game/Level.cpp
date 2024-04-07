// Author: Jake Rieger
// Created: 4/4/2024.
//

#include "Level.h"
#include "Logger.h"
#include "Engine/PhysicsContext.h"

#ifndef _WIN32
    #include <bits/ranges_algo.h>
#endif

using Packer::Schemas::Sprite;

static constexpr f32 k_TileSize = 2.f;
static std::unordered_map<i32, Sprite*> g_SpriteMap;
static b2PolygonShape g_BoxCollider;
static b2EdgeShape g_RampLeftCollider;
static b2EdgeShape g_RampRightCollider;

static void LoadSpriteMap() {
    g_SpriteMap[1] = &Resources::GetResource<Sprite>(Resources::ResourceType::Sprite, "floor.png");
    g_SpriteMap[2] =
      &Resources::GetResource<Sprite>(Resources::ResourceType::Sprite, "ramp_left.png");
    g_SpriteMap[3] =
      &Resources::GetResource<Sprite>(Resources::ResourceType::Sprite, "ramp_right.png");
    g_SpriteMap[4] =
      &Resources::GetResource<Sprite>(Resources::ResourceType::Sprite, "button_unpressed.png");
    g_SpriteMap[5] =
      &Resources::GetResource<Sprite>(Resources::ResourceType::Sprite, "button_pressed.png");
}

static void LoadColliderMap() {
    g_BoxCollider.SetAsBox(2.f, 2.f);

    // TODO: Figure out why this doesn't appear to be the right shape
    b2Vec2 v1(0.f, 0.f);
    b2Vec2 v2(2.f, 2.f);

    g_RampLeftCollider.SetTwoSided(v1, v2);
}

Level::Level(const std::string& name, const FLevel& levelData) : IGameObject(name) {
    assert(levelData.Tiles.size() == levelData.Columns * levelData.Rows);

    LoadSpriteMap();
    LoadColliderMap();

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
                if (id == 1) {
                    body->CreateFixture(&g_BoxCollider, 0.f);
                } else if (id == 2) {
                    body->CreateFixture(&g_RampRightCollider, 0.f);
                } else if (id == 3) {
                    body->CreateFixture(&g_RampLeftCollider, 0.f);
                }
            }
        }
    }

    // Create a tilemap for each tile
    for (auto& [id, positions] : m_TilePositions) {
        auto tilemap = new ATilemap(*g_SpriteMap[id], positions, k_TileSize);
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