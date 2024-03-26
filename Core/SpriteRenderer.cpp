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

ASpriteRenderer::ASpriteRenderer(unsigned char* data, int width, int height, int channels) {
    m_Quad     = std::make_unique<AQuad>();
    m_SpriteId = Utilities::LoadTextureFromData(data, width, height, channels);
}

void ASpriteRenderer::Start(FSceneContext& sceneContext) {
    IComponent::Start(sceneContext);
    m_Quad->Init();
}

void ASpriteRenderer::Update(float deltaTime, FSceneContext& sceneContext) {
    IComponent::Update(deltaTime, sceneContext);
}

void ASpriteRenderer::Draw(FSceneContext& sceneContext, const ATransform* transform) const {
    const auto projection = glm::ortho(0.f,
                                       static_cast<float>(Graphics::GetWindowSize().at(0)),
                                       static_cast<float>(Graphics::GetWindowSize().at(1)),
                                       0.f,
                                       -1.f,
                                       1.f);
    m_Quad->Draw(m_SpriteId, projection, transform->GetModelMatrix());
}

void ASpriteRenderer::Destroyed(FSceneContext& sceneContext) {
    IComponent::Destroyed(sceneContext);
    m_Quad->Cleanup();
}