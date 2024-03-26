// Author: Jake Rieger
// Created: 3/25/2024.
//

#include "Nugget.h"
#include "Engine/Resources.h"

Nugget::Nugget(const std::string& name) : IGameObject(name) {
    const auto nuggetSprite =
      Resources::GetResource<Packer::Schemes::Sprite>(Resources::ResourceType::Sprite,
                                                      "nugget.png");
    m_SpriteRenderer = new ASpriteRenderer(nuggetSprite->data,
                                           nuggetSprite->width,
                                           nuggetSprite->height,
                                           nuggetSprite->channels);
    m_SpriteRenderer->SetParent(this);
    RegisterComponent(m_SpriteRenderer);
}

void Nugget::Draw(FSceneContext& sceneContext) {
    m_SpriteRenderer->Draw(sceneContext, GetTransform());
}

void Nugget::Start(FSceneContext& sceneContext) {
    IGameObject::Start(sceneContext);
    m_SpriteRenderer->Start(sceneContext);
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