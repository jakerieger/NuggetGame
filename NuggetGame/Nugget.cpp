// Author: Jake Rieger
// Created: 3/25/2024.
//

#include "Nugget.h"
#include "Camera.h"
#include "Utilities.inl"
#include "Engine/AudioContext.h"
#include "Engine/InputCodes.h"
#include "Engine/Resources.h"
#include "Cursor.h"

Nugget::Nugget(const std::string& name, const glm::vec2& startPosition)
    : IGameObject(name), m_StartPosition(startPosition) {}

void Nugget::Initialize() {
    m_SpriteRenderer = new ASpriteRenderer(this);
    m_SpriteRenderer->SetParent(this);

    m_Rigidbody = new ARigidbody(100.f, 1.f, ColliderShape::Circle);
    m_Rigidbody->SetParent(this);

    m_Transform.SetScale(2.f, 2.f);
    m_Transform.SetPosition(m_StartPosition.x, m_StartPosition.y);

    RegisterComponent(m_SpriteRenderer);
    RegisterComponent(m_Rigidbody);
}

void Nugget::Draw(FSceneContext& sceneContext) {
    m_SpriteRenderer->Draw(sceneContext, GetTransform());
}

void Nugget::Start(FSceneContext& sceneContext) {
    IGameObject::Start(sceneContext);
    m_SpriteRenderer->Start(sceneContext);
    m_Rigidbody->Start(sceneContext);

    Camera::Follow(this);
    Cursor::Hide();
}

void Nugget::Update(float deltaTime, FSceneContext& sceneContext) {
    IGameObject::Update(deltaTime, sceneContext);
    m_SpriteRenderer->Update(deltaTime, sceneContext);

    if (m_Rigidbody->IsGrounded() && m_Falling) {
        m_Falling   = false;
        m_JumpCount = 0;
        // Audio::PlayOneShot(
        //   (Utilities::JoinPath(Resources::GetRoot(), "Assets", "audio", "land.wav")).string(),
        //   "LandSound",
        //   Audio::EAudioTag::FX);
    }
}

void Nugget::FixedUpdate(FSceneContext& sceneContext) {
    IGameObject::FixedUpdate(sceneContext);
    m_Rigidbody->FixedUpdate(sceneContext);
}

void Nugget::Destroyed(FSceneContext& sceneContext) {
    IGameObject::Destroyed(sceneContext);
    m_SpriteRenderer->Destroyed(sceneContext);
    m_Rigidbody->Destroyed(sceneContext);
    delete m_SpriteRenderer;
    delete m_Rigidbody;
}

void Nugget::OnKeyDown(FKeyEvent& event) {
    IGameObject::OnKeyDown(event);

    if (event.KeyCode == KeyCode::Space) {
        if (m_Rigidbody->IsGrounded() || m_JumpCount < 2) {
            m_Rigidbody->AddImpulse({0.f, 5000.f});
            m_Falling = true;
            m_JumpCount++;
            // Play jump sound effect
            const auto sfxPath =
                Utilities::JoinPath(Resources::GetRoot(), "Assets", "audio", "jump.wav");
            // Audio::PlayOneShot(sfxPath.string(), "JumpSound", Audio::EAudioTag::FX);
        }
    }

    if (event.KeyCode == KeyCode::F) {
        // Interact event, use for buttons and stuff
    }
}

void Nugget::OnKey(FKeyEvent& event) {
    IGameObject::OnKey(event);

    if (event.KeyCode == KeyCode::D || event.KeyCode == KeyCode::Right) {
        if (m_Rigidbody->IsGrounded()) {
            m_Rigidbody->AddForce({2000.f, 0.f});
        } else {
            m_Rigidbody->AddForce({1000.f, 0.f});
        }
    }

    if (event.KeyCode == KeyCode::A || event.KeyCode == KeyCode::Left) {
        if (m_Rigidbody->IsGrounded()) {
            m_Rigidbody->AddForce({-2000.f, 0.f});
        } else {
            m_Rigidbody->AddForce({-1000.f, 0.f});
        }
    }
}