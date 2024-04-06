// Author: Jake Rieger
// Created: 4/4/2024.
//

#include "Level.h"

#include "Logger.h"

Level::Level(const std::string& name, const FLevel& levelData) : IGameObject(name) {
    assert(levelData.Tiles.size() == levelData.Columns * levelData.Rows);

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
}

void Level::Start(FSceneContext& sceneContext) {
    IGameObject::Start(sceneContext);
}

void Level::Update(const float deltaTime, FSceneContext& sceneContext) {
    IGameObject::Update(deltaTime, sceneContext);
}

void Level::Draw(FSceneContext& sceneContext) {}

void Level::Destroyed(FSceneContext& sceneContext) {
    IGameObject::Destroyed(sceneContext);
}