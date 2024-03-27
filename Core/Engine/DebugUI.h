//
// Created by conta on 2/15/2024.
//

#pragma once
#include "Scene.h"

namespace Debug::UI {
    void Initialize();
    void Update(float deltaTime, AScene* activeScene);
    void Draw();
    void Shutdown();

    void ToggleVisible();
}  // namespace Debug::UI