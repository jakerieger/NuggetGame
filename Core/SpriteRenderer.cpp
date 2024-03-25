// Author: Jake Rieger
// Created: 3/25/2024.
//

#include "SpriteRenderer.h"
#include "Utilities.inl"

#include <memory>

ASpriteRenderer::ASpriteRenderer(const char* resource) {
    // Do some shit here
    m_Quad     = std::make_unique<AQuad>();
    m_SpriteId = Utilities::LoadTexture(resource);
}

void ASpriteRenderer::Start(FSceneContext& sceneContext) {
    IComponent::Start(sceneContext);
    m_Quad->Init();
}

void ASpriteRenderer::Update(float deltaTime, FSceneContext& sceneContext) {
    IComponent::Update(deltaTime, sceneContext);
}

void ASpriteRenderer::Draw(FSceneContext& sceneContext, const ATransform* transform) const {
    m_Quad->Draw(m_SpriteId);
}

void ASpriteRenderer::Destroyed(FSceneContext& sceneContext) {
    IComponent::Destroyed(sceneContext);
    m_Quad->Cleanup();
}