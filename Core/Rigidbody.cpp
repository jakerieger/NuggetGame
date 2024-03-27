// Author: Jake Rieger
// Created: 3/26/2024.
//

#include "Rigidbody.h"

#include "Engine/PhysicsContext.h"
#include "Interfaces/GameObject.h"

ARigidbody::ARigidbody() : m_Body(nullptr) {}

void ARigidbody::Start(FSceneContext& sceneContext) {
    IComponent::Start(sceneContext);

    const auto transform = GetParent()->GetTransform();
    const auto position  = transform->GetPosition();
    const auto scale     = transform->GetScale();

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x, position.y);
    m_Body = Physics::GetWorld()->CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(scale.x / 2.f, scale.y / 2.f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape    = &dynamicBox;
    fixtureDef.density  = 0.01f;
    fixtureDef.friction = 0.1f;
    m_Body->CreateFixture(&fixtureDef);
}

void ARigidbody::Update(float deltaTime, FSceneContext& sceneContext) {
    IComponent::Update(deltaTime, sceneContext);
    GetParent()->GetTransform()->SetPosition(m_Body->GetPosition().x, m_Body->GetPosition().y);
}

void ARigidbody::Destroyed(FSceneContext& sceneContext) { IComponent::Destroyed(sceneContext); }

void ARigidbody::AddImpulse(glm::vec2 impulse) const {
    const auto transform = GetParent()->GetTransform();
    const auto position  = transform->GetPosition();
    m_Body->ApplyLinearImpulse({impulse.x, impulse.y}, {position.x, position.y}, true);
}