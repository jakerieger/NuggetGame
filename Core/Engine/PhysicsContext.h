// Author: Jake Rieger
// Created: 3/26/2024.
//

#pragma once

#include <box2d/box2d.h>

namespace Physics {
    void Init();
    void Tick(float timeStep);
    b2World* GetWorld();
    void Shutdown();
}  // namespace Physics