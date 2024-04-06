// Author: Jake Rieger
// Created: 4/4/2024.
//

#pragma once

#include "STL.h"
#include "Components/Tilemap.h"
#include "Interfaces/GameObject.h"
#include "Interfaces/Drawable.h"
#include "Engine/Resources.h"

struct FLevel {
    std::vector<i32> Tiles;
    u32 Rows;
    u32 Columns;
    glm::vec2 PlayerStart;
    glm::vec2 ObjectivePosition;
};

class Level final : public IGameObject,
                    public IDrawable {
public:
    Level(const std::string& name, const FLevel& levelData);

    void Start(FSceneContext& sceneContext) override;
    void Update(float deltaTime, FSceneContext& sceneContext) override;
    void Draw(FSceneContext& sceneContext) override;
    void Destroyed(FSceneContext& sceneContext) override;

private:
    // std::unordered_map<i32, ATilemap*> m_Tilemaps;
    std::vector<std::vector<i32>> m_TileIds;
    std::unordered_map<i32, std::vector<glm::vec2>> m_TilePositions;
    std::vector<ATilemap*> m_Tilemaps;
    std::unordered_map<i32, Packer::Schemas::Sprite*> m_SpriteMap;
};