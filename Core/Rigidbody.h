// Author: Jake Rieger
// Created: 3/26/2024.
//

#pragma once

#include "Interfaces/Component.h"

#include <box2d/b2_body.h>
#include <glm/vec2.hpp>

class ARigidbody final : public IComponent {
public:
    ARigidbody();

    void Start(FSceneContext& sceneContext) override;
    void FixedUpdate(FSceneContext& sceneContext) override;
    void Destroyed(FSceneContext& sceneContext) override;

    void AddImpulse(glm::vec2 impulse) const;
    void AddTorque(float torque) const;

private:
    b2Body* m_Body;
};
