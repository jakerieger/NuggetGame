// Author: Jake Rieger
// Created: 3/25/2024.
//

#include "SpriteRenderer.h"

#include "Camera.h"
#include "Utilities.inl"
#include "SceneContext.h"

#include <memory>

ASpriteRenderer::ASpriteRenderer(const char* resource) {
    m_Sprite   = std::make_unique<ASprite>();
    m_SpriteId = Utilities::LoadTextureFromFile(resource);
}

ASpriteRenderer::ASpriteRenderer(const Packer::Schemas::Sprite& sprite) {
    m_Sprite = std::make_unique<ASprite>();
    m_SpriteId =
      Utilities::LoadTextureFromData(sprite.data, sprite.width, sprite.height, sprite.channels);
}

void ASpriteRenderer::Start(FSceneContext& sceneContext) {
    IComponent::Start(sceneContext);
    m_Sprite->Init();
}

void ASpriteRenderer::Update(float deltaTime, FSceneContext& sceneContext) {
    IComponent::Update(deltaTime, sceneContext);
}

void ASpriteRenderer::Draw(FSceneContext& sceneContext, const ATransform* transform) const {
    m_Sprite->Draw(m_SpriteId, Camera::BuildProjectionMatrix(0.f), transform->GetModelMatrix());
}

void ASpriteRenderer::Destroyed(FSceneContext& sceneContext) {
    IComponent::Destroyed(sceneContext);
    m_Sprite->Cleanup();
}