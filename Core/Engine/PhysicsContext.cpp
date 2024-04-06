// Author: Jake Rieger
// Created: 3/26/2024.
//

#include "PhysicsContext.h"

#include "Camera.h"
#include "GraphicsContext.h"
#include "Logger.h"
#include "STL.h"

#include <glm/vec2.hpp>

namespace Physics {
    static constexpr float DEFAULT_GRAVITY = -9.8f;
    b2World g_World({0.f, DEFAULT_GRAVITY});

    void Initialize(const b2Vec2& gravity) {
        g_World.SetGravity(gravity);

        // TODO: Set up KillY volume

        Logger::LogInfo(Logger::Subsystems::PHYSICS, "Physics subsystem initialized.");
    }

    void Tick(const float timeStep) {
        static constexpr i32 VELOCITY_ITERS = 6;
        static constexpr i32 POSITION_ITERS = 2;
        g_World.Step(timeStep, VELOCITY_ITERS, POSITION_ITERS);
    }

    b2World* GetWorld() {
        return &g_World;
    }

    void Shutdown() {}
}  // namespace Physics