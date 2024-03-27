// Author: Jake Rieger
// Created: 3/25/2024.
//

#include "Nugget.h"

#include "InputCodes.h"
#include "Engine/Resources.h"

Nugget::Nugget(const std::string& name) : IGameObject(name) {
    const auto nuggetSprite =
      Resources::GetResource<Packer::Schemas::Sprite>(Resources::ResourceType::Sprite,
                                                      "nugget.png");
    m_SpriteRenderer = new ASpriteRenderer(nuggetSprite);
    m_SpriteRenderer->SetParent(this);

    m_Rigidbody = new ARigidbody();
    m_Rigidbody->SetParent(this);

    RegisterComponent(m_SpriteRenderer);
    RegisterComponent(m_Rigidbody);
}

void Nugget::Draw(FSceneContext& sceneContext) {
    m_SpriteRenderer->Draw(sceneContext, GetTransform());
}

void Nugget::Start(FSceneContext& sceneContext) {
    IGameObject::Start(sceneContext);
    m_SpriteRenderer->Start(sceneContext);
    m_Transform.SetScale(16.f, 16.f);
    m_Transform.SetPosition(640.f / 2.f, 360.f / 2.f);
    m_Rigidbody->Start(sceneContext);
}

void Nugget::Update(float deltaTime, FSceneContext& sceneContext) {
    IGameObject::Update(deltaTime, sceneContext);
    m_SpriteRenderer->Update(deltaTime, sceneContext);
    m_Rigidbody->Update(deltaTime, sceneContext);
}

void Nugget::Destroyed(FSceneContext& sceneContext) {
    IGameObject::Destroyed(sceneContext);
    m_SpriteRenderer->Destroyed(sceneContext);
    delete m_SpriteRenderer;
}

void Nugget::OnKeyDown(FKeyEvent& event) {
    IGameObject::OnKeyDown(event);

    if (event.KeyCode == KeyCode::Space) {
        m_Rigidbody->AddImpulse({0.f, 100.f});
    }

    if (event.KeyCode == KeyCode::F) {
        // Interact event, use for buttons and stuff
    }
}

void Nugget::OnKey(FKeyEvent& event) {
    IGameObject::OnKey(event);

    if (event.KeyCode == KeyCode::D) {
        m_Rigidbody->AddTorque(-10000.f);
    }

    if (event.KeyCode == KeyCode::A) {
        m_Rigidbody->AddTorque(10000.f);
    }
}