// Author: Jake Rieger
// Created: 3/29/2024.
//

#include "Camera.h"

#include "GraphicsContext.h"

FCamera::FCamera() {
    m_Width  = Graphics::GetWindowSize().at(0);
    m_Height = Graphics::GetWindowSize().at(1);
    ResetView();
}

void FCamera::ResetView() {
    m_Center = {0.f, 25.f};
    m_Zoom   = 1.f;
}

glm::vec2 FCamera::ScreenToWorld(const glm::vec2& screenPoint) const {
    const f32 w = static_cast<f32>(m_Width);
    const f32 h = static_cast<f32>(m_Height);
    const f32 u = screenPoint.x / w;
    const f32 v = (h - screenPoint.y) / h;

    const f32 ratio = w / h;
    glm::vec2 extents(ratio * 25.0f, 25.0f);
    extents *= m_Zoom;

    const glm::vec2 lower = m_Center - extents;
    const glm::vec2 upper = m_Center + extents;

    glm::vec2 pw;
    pw.x = (1.0f - u) * lower.x + u * upper.x;
    pw.y = (1.0f - v) * lower.y + v * upper.y;
    return pw;
}

glm::vec2 FCamera::WorldToScreen(const glm::vec2& worldPoint) const {
    const f32 w       = static_cast<f32>(m_Width);
    const f32 h       = static_cast<f32>(m_Height);
    const float ratio = w / h;
    glm::vec2 extents(ratio * 25.0f, 25.0f);
    extents *= m_Zoom;

    const glm::vec2 lower = m_Center - extents;
    const glm::vec2 upper = m_Center + extents;

    float u = (worldPoint.x - lower.x) / (upper.x - lower.x);
    float v = (worldPoint.y - lower.y) / (upper.y - lower.y);

    glm::vec2 ps;
    ps.x = u * w;
    ps.y = (1.0f - v) * h;
    return ps;
}

void FCamera::BuildProjectionMatrix(glm::mat4& m, const f32 zBias) const {
    const f32 w       = static_cast<f32>(m_Width);
    const f32 h       = static_cast<f32>(m_Height);
    const float ratio = w / h;
    glm::vec2 extents(ratio * 25.0f, 25.0f);
    extents *= m_Zoom;

    const glm::vec2 lower = m_Center - extents;
    const glm::vec2 upper = m_Center + extents;

    const glm::vec4 a = {2.0f / (upper.x - lower.x), 0.f, 0.f, 0.f};
    const glm::vec4 b = {0.f, 2.0f / (upper.y - lower.y), 0.f, 0.f};
    const glm::vec4 c = {0.f, 0.f, 1.0f, 0.f};
    const glm::vec4 d = {-(upper.x + lower.x) / (upper.x - lower.x),
                         -(upper.y + lower.y) / (upper.y - lower.y),
                         zBias,
                         1.f};

    m[0] = a;
    m[1] = b;
    m[2] = c;
    m[3] = d;
}

void FCamera::Update(const i32 width, const i32 height) {
    m_Width  = width;
    m_Height = height;
}

namespace Camera {
    glm::vec2 ScreenToWorld(const glm::vec2& screenPoint, const glm::vec2& viewport) {
        const glm::vec2 center = {0.f, 25.f};
        constexpr f32 zoom     = 1.f;
        const f32 u            = screenPoint.x / viewport.x;
        const f32 v            = (viewport.y - screenPoint.y) / viewport.y;

        const f32 ratio = viewport.x / viewport.y;
        glm::vec2 extents(ratio * 25.0f, 25.0f);
        extents *= zoom;

        const glm::vec2 lower = center - extents;
        const glm::vec2 upper = center + extents;

        glm::vec2 pw;
        pw.x = (1.0f - u) * lower.x + u * upper.x;
        pw.y = (1.0f - v) * lower.y + v * upper.y;
        return pw;
    }

    glm::vec2 WorldToScreen(const glm::vec2& worldPoint, const glm::vec2& viewport) {
        const glm::vec2 center = {0.f, 25.f};
        constexpr f32 zoom     = 1.f;
        const float ratio      = viewport.x / viewport.y;
        glm::vec2 extents(ratio * 25.0f, 25.0f);
        extents *= zoom;

        const glm::vec2 lower = center - extents;
        const glm::vec2 upper = center + extents;

        float u = (worldPoint.x - lower.x) / (upper.x - lower.x);
        float v = (worldPoint.y - lower.y) / (upper.y - lower.y);

        glm::vec2 ps;
        ps.x = u * viewport.x;
        ps.y = (1.0f - v) * viewport.y;
        return ps;
    }
}  // namespace Camera
