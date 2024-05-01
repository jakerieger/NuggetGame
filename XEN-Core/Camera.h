// Author: Jake Rieger
// Created: 3/29/2024.
//

#pragma once

#include "STL.h"

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

class IGameObject;

namespace Camera {
    void Initialize();
    void ResetView();
    glm::mat4 BuildProjectionMatrix(f32 zBias);
    glm::mat4 BuildViewMatrix();
    void Update();
    void Follow(IGameObject* target);

    glm::vec2 ScreenToWorld(const glm::vec2& screenPoint);
    glm::vec2 WorldToScreen(const glm::vec2& worldPoint);
}  // namespace Camera