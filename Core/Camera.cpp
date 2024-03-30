// Author: Jake Rieger
// Created: 3/29/2024.
//

#include "Camera.h"
#include "Engine/GraphicsContext.h"

namespace Camera {
    glm::vec2 g_Center {};
    f32 g_Zoom {};
    f32 g_Width;
    f32 g_Height;

    void Initialize() {
        Update();
        ResetView();
    }

    void ResetView() {
        g_Center = {0.f, 25.f};
        g_Zoom   = 1.f;
    }

    void Update() {
        g_Width  = Graphics::GetWindowSizeGLM().x;
        g_Height = Graphics::GetWindowSizeGLM().y;
    }

    glm::vec2 ScreenToWorld(const glm::vec2& screenPoint) {
        const f32 u = screenPoint.x / g_Width;
        const f32 v = (g_Height - screenPoint.y) / g_Height;

        const f32 ratio = g_Width / g_Height;
        glm::vec2 extents(ratio * 25.0f, 25.0f);
        extents *= g_Zoom;

        const glm::vec2 lower = g_Center - extents;
        const glm::vec2 upper = g_Center + extents;

        glm::vec2 pw;
        pw.x = (1.0f - u) * lower.x + u * upper.x;
        pw.y = (1.0f - v) * lower.y + v * upper.y;
        return pw;
    }

    glm::vec2 WorldToScreen(const glm::vec2& worldPoint) {
        const f32 ratio = g_Width / g_Height;
        glm::vec2 extents(ratio * 25.0f, 25.0f);
        extents *= g_Zoom;

        const glm::vec2 lower = g_Center - extents;
        const glm::vec2 upper = g_Center + extents;

        const float u = (worldPoint.x - lower.x) / (upper.x - lower.x);
        const float v = (worldPoint.y - lower.y) / (upper.y - lower.y);

        glm::vec2 ps;
        ps.x = u * g_Width;
        ps.y = (1.0f - v) * g_Height;
        return ps;
    }

    glm::mat4 BuildProjectionMatrix(f32 zBias) {
        const f32 w       = g_Width;
        const f32 h       = g_Height;
        const float ratio = w / h;
        glm::vec2 extents(ratio * 25.0f, 25.0f);
        extents *= g_Zoom;

        const glm::vec2 lower = g_Center - extents;
        const glm::vec2 upper = g_Center + extents;

        const glm::vec4 a = {2.0f / (upper.x - lower.x), 0.f, 0.f, 0.f};
        const glm::vec4 b = {0.f, 2.0f / (upper.y - lower.y), 0.f, 0.f};
        const glm::vec4 c = {0.f, 0.f, 1.0f, 0.f};
        const glm::vec4 d = {-(upper.x + lower.x) / (upper.x - lower.x),
                             -(upper.y + lower.y) / (upper.y - lower.y),
                             zBias,
                             1.f};

        glm::mat4 m(1.f);
        m[0] = a;
        m[1] = b;
        m[2] = c;
        m[3] = d;

        return m;
    }

}  // namespace Camera
