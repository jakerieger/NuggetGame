// Author: Jake Rieger
// Created: 3/25/2024.
//

#include "Nugget.h"

#include "Engine/GraphicsContext.h"
#include "Engine/Resources.h"

Nugget::Nugget(const std::string& name) : IGameObject(name) {
    const auto nuggetSprite =
      Resources::GetResource<Packer::Schemas::Sprite>(Resources::ResourceType::Sprite,
                                                      "nugget.png");
    m_SpriteRenderer = new ASpriteRenderer(nuggetSprite);
    m_SpriteRenderer->SetParent(this);
    RegisterComponent(m_SpriteRenderer);
}

void Nugget::Draw(FSceneContext& sceneContext) {
    m_SpriteRenderer->Draw(sceneContext, GetTransform());
}

void Nugget::Start(FSceneContext& sceneContext) {
    IGameObject::Start(sceneContext);
    m_SpriteRenderer->Start(sceneContext);
    m_Transform.SetScale(16.f, 16.f);
    m_Transform.SetPosition(640.f / 2.f, 360.f / 2.f);
}

void Nugget::Update(float deltaTime, FSceneContext& sceneContext) {
    IGameObject::Update(deltaTime, sceneContext);
    m_SpriteRenderer->Update(deltaTime, sceneContext);
}

void Nugget::Destroyed(FSceneContext& sceneContext) {
    IGameObject::Destroyed(sceneContext);
    m_SpriteRenderer->Destroyed(sceneContext);
    delete m_SpriteRenderer;
}