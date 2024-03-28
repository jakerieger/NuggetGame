// Author: Jake Rieger
// Created: 3/26/2024.
//

#pragma once

#include "Types.h"
#include "Interfaces/Component.h"

#include <box2d/b2_body.h>
#include <glm/vec2.hpp>

enum class ColliderShape : u8 { Circle, Box, Polygon };

class ARigidbody final : public IComponent {
public:
    ARigidbody(f32 density, f32 friction, ColliderShape shape);

    void Start(FSceneContext& sceneContext) override;
    void FixedUpdate(FSceneContext& sceneContext) override;
    void Destroyed(FSceneContext& sceneContext) override;

    void AddImpulse(glm::vec2 impulse) const;
    void AddTorque(float torque) const;

private:
    b2Body* m_Body;
    f32 m_Density;
    f32 m_Friction;
    ColliderShape m_Shape;
};
