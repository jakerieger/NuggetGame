// Author: Jake Rieger
// Created: 3/29/2024.
//

#pragma once

#include "STL.h"

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

struct FCamera {
    FCamera();

    void ResetView();
    [[nodiscard]] glm::vec2 ScreenToWorld(const glm::vec2& screenPoint) const;
    [[nodiscard]] glm::vec2 WorldToScreen(const glm::vec2& worldPoint) const;
    void BuildProjectionMatrix(glm::mat4& m, f32 zBias) const;
    void Update(i32 width, i32 height);

    glm::vec2 m_Center {};
    f32 m_Zoom {};
    i32 m_Width;
    i32 m_Height;
};

namespace Camera {
    glm::vec2 ScreenToWorld(const glm::vec2& screenPoint, const glm::vec2& viewport);
    glm::vec2 WorldToScreen(const glm::vec2& worldPoint, const glm::vec2& viewport);
}  // namespace Camera