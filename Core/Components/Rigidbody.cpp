// Author: Jake Rieger
// Created: 3/26/2024.
//

#include "Rigidbody.h"
#include "Engine/PhysicsContext.h"
#include "Interfaces/GameObject.h"

#include <stdexcept>

ARigidbody::ARigidbody(const f32 density, const f32 friction, const ColliderShape shape)
    : m_Body(nullptr), m_Density(density), m_Friction(friction), m_Shape(shape) {}

void ARigidbody::Start(FSceneContext& sceneContext) {
    IComponent::Start(sceneContext);

    const auto transform = GetParent()->GetTransform();
    const auto position  = transform->GetPosition();
    const auto scale     = transform->GetScale();

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x, position.y);
    m_Body = Physics::GetWorld()->CreateBody(&bodyDef);

    // if (m_Shape == ColliderShape::Box) {
    //     b2PolygonShape box;
    //     box.SetAsBox(scale.x / 2.f, scale.y / 2.f);
    //     m_b2Shape = std::move(&box);
    // } else if (m_Shape == ColliderShape::Circle) {
    //     b2CircleShape circle;
    //     circle.m_p.Set(0.f, 0.f);
    //     circle.m_radius = scale.x / 2.f;
    //     m_b2Shape       = std::move(&circle);
    // } else if (m_Shape == ColliderShape::Polygon) {
    //     throw std::runtime_error("Not yet implemented");
    // }

    b2CircleShape circle;
    circle.m_p.Set(0.f, 0.f);
    circle.m_radius         = scale.x / 2.f;
    m_b2FixtureDef.shape    = &circle;
    m_b2FixtureDef.density  = m_Density;
    m_b2FixtureDef.friction = m_Friction;
    m_Body->CreateFixture(&m_b2FixtureDef);
}

void ARigidbody::FixedUpdate(FSceneContext& sceneContext) {
    IComponent::FixedUpdate(sceneContext);

    const auto transform = GetParent()->GetTransform();
    transform->SetPosition(m_Body->GetPosition().x, m_Body->GetPosition().y);
    transform->SetRotation(glm::degrees(m_Body->GetAngle()));
}

void ARigidbody::Destroyed(FSceneContext& sceneContext) {
    IComponent::Destroyed(sceneContext);
    Physics::GetWorld()->DestroyBody(m_Body);
}

void ARigidbody::AddImpulse(glm::vec2 impulse) const {
    const auto transform = GetParent()->GetTransform();
    const auto position  = transform->GetPosition();
    m_Body->ApplyLinearImpulse({impulse.x, impulse.y}, {position.x, position.y}, true);
}

void ARigidbody::AddTorque(const float torque) const {
    m_Body->ApplyTorque(torque, true);
}

void ARigidbody::AddForce(const b2Vec2& force) const {
    m_Body->ApplyForceToCenter(force, true);
}

bool ARigidbody::IsGrounded() const {
    static constexpr f32 TOLERANCE = 0.05f;
    if (m_Body->GetLinearVelocity().y < -TOLERANCE || m_Body->GetLinearVelocity().y > TOLERANCE)
        return false;

    return true;
}