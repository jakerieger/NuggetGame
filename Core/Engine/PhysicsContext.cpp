// Author: Jake Rieger
// Created: 3/26/2024.
//

#include "PhysicsContext.h"
#include "STL.h"

namespace Physics {
    static constexpr float GRAVITY = -9.8f;
    b2World g_World({0.f, GRAVITY});

    void Init() {
        // Floor
        {
            b2BodyDef groundbodyDef;
            groundbodyDef.position.Set(0.f, 5.f);

            const auto groundBody = g_World.CreateBody(&groundbodyDef);
            b2PolygonShape groundBox;
            groundBox.SetAsBox(1280.f / 2.f, 0.f);

            groundBody->CreateFixture(&groundBox, 0.f);
        }

        // Right wall
        {
            b2BodyDef rWallDef;
            rWallDef.position.Set(644.f, 360.f);

            const auto rWallBody = g_World.CreateBody(&rWallDef);
            b2PolygonShape rWallBox;
            rWallBox.SetAsBox(10.f, 720.f);

            rWallBody->CreateFixture(&rWallBox, 0.f);
        }

        // Left wall
        {
            b2BodyDef lWallDef;
            lWallDef.position.Set(-5.f, 720.f / 2.f);

            const auto lWallBody = g_World.CreateBody(&lWallDef);
            b2PolygonShape lWallBox;
            lWallBox.SetAsBox(10.f, 720.f);

            lWallBody->CreateFixture(&lWallBox, 0.f);
        }
    }

    void Tick(const float timeStep) {
        static constexpr i32 VELOCITY_ITERS = 6;
        static constexpr i32 POSITION_ITERS = 2;
        g_World.Step(timeStep, VELOCITY_ITERS, POSITION_ITERS);
    }

    b2World* GetWorld() { return &g_World; }

    void Shutdown() {}
}  // namespace Physics