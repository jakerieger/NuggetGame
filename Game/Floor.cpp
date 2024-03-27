// Author: Jake Rieger
// Created: 3/26/2024.
//

#include "Floor.h"

Floor::Floor(const std::string& name, const glm::vec2& pos) : IGameObject(name) {
    const auto floorSprite =
      Resources::GetResource<Packer::Schemas::Sprite>(Resources::ResourceType::Sprite, "floor.png");
    m_SpriteRenderer = new ASpriteRenderer(floorSprite);
    m_SpriteRenderer->SetParent(this);

    m_Transform.SetPosition(pos);

    RegisterComponent(m_SpriteRenderer);
}

void Floor::Start(FSceneContext& sceneContext) {
    IGameObject::Start(sceneContext);
    m_SpriteRenderer->Start(sceneContext);
}

void Floor::Update(const float deltaTime, FSceneContext& sceneContext) {
    IGameObject::Update(deltaTime, sceneContext);
    m_SpriteRenderer->Update(deltaTime, sceneContext);
}

void Floor::Draw(FSceneContext& sceneContext) {
    m_Transform.SetScale(16.f, 16.f);
    m_SpriteRenderer->Draw(sceneContext, GetTransform());
}

void Floor::Destroyed(FSceneContext& sceneContext) {
    IGameObject::Destroyed(sceneContext);
    m_SpriteRenderer->Destroyed(sceneContext);
}
