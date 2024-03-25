// Author: Jake Rieger
// Created: 3/25/2024.
//

#include "Nugget.h"
#include "Engine/Resources.h"

Nugget::Nugget(const std::string& name) : IGameObject(name) {
    m_SpriteRenderer =
      new ASpriteRenderer(Resources::GetResource(RES_SPRITE, "nugget.png").c_str());
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