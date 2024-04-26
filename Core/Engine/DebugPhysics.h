//
// Created by jr on 3/27/24.
//

#pragma once

#include <box2d/b2_draw.h>
#include <box2d/b2_math.h>

/// Credit for this module goes to box2d,
/// implemented from the testbed source.
/// See:\link https://github.com/erincatto/box2d/blob/main/testbed/draw.h \endlink
namespace Debug::Physics {
    struct b2AABB;
    struct GLRenderPoints;
    struct GLRenderLines;
    struct GLRenderTriangles;

    class ADebugDraw final : public b2Draw {
    public:
        ADebugDraw();
        ~ADebugDraw();

        void Initialize();
        void Shutdown();

        void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
        void
        DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
        void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;
        void DrawSolidCircle(const b2Vec2& center,
                             float radius,
                             const b2Vec2& axis,
                             const b2Color& color) override;
        void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
        void DrawTransform(const b2Transform& xf) override;
        void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;
        void DrawString(int x, int y, const char* string, ...);
        void DrawString(const b2Vec2& p, const char* string, ...);
        void DrawAABB(b2AABB* aabb, const b2Color& color);

        void Flush();

        GLRenderLines* m_Points;
        GLRenderLines* m_Lines;
        GLRenderTriangles* m_Triangles;
    };

    extern ADebugDraw g_DebugDraw;
}  // namespace Debug::Physics