//
// Created by jr on 2/16/24.
//

#pragma once

#include "Interfaces/GameObject.h"
#include "STL.h"
#include "Camera.h"

struct FSceneContext {
    std::vector<std::unique_ptr<IGameObject>> GameObjects;
    FCamera Camera;
};