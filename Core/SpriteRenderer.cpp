// Author: Jake Rieger
// Created: 3/25/2024.
//

#include "SpriteRenderer.h"
#include "Utilities.inl"

#include <memory>

ASpriteRenderer::ASpriteRenderer(const char* resource) {
    m_Quad     = std::make_unique<AQuad>();
    m_SpriteId = Utilities::LoadTextureFromFile(resource);
}

ASpriteRenderer::ASpriteRenderer(const Packer::Schemas::Sprite& sprite) {
    m_Quad = std::make_unique<AQuad>();
    m_SpriteId =
      Utilities::LoadTextureFromData(sprite.data, sprite.width, sprite.height, sprite.channels);
}

void ASpriteRenderer::Start(FSceneContext& sceneContext) {
    IComponent::Start(sceneContext);
    m_Quad->Init();
}

void ASpriteRenderer::Update(float deltaTime, FSceneContext& sceneContext) {
    IComponent::Update(deltaTime, sceneContext);
}

void ASpriteRenderer::Draw(FSceneContext& sceneContext, const ATransform* transform) const {
    const auto projection = glm::ortho(0.f, 640.f, 0.f, 360.f);
    m_Quad->Draw(m_SpriteId, projection, transform->GetModelMatrix());
}

void ASpriteRenderer::Destroyed(FSceneContext& sceneContext) {
    IComponent::Destroyed(sceneContext);
    m_Quad->Cleanup();
}