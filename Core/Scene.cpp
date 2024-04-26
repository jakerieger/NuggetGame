//
// Created by jr on 2/16/24.
//

#include "Scene.h"
#include "Engine/GraphicsContext.h"
#include "Interfaces/Drawable.h"

#include <utility>
#include <glad/glad.h>

AScene::AScene(std::string name) : m_Name(std::move(name)) {}

void AScene::Awake() {
    for (const auto& go : m_SceneContext.GameObjects) {
        go->Awake(m_SceneContext);
    }
}

void AScene::Start() {
    for (const auto& go : m_SceneContext.GameObjects) {
        go->Start(m_SceneContext);
    }
}

void AScene::Update(const float deltaTime) {
    for (const auto& go : m_SceneContext.GameObjects) {
        go->Update(deltaTime, m_SceneContext);
    }
}

void AScene::LateUpdate() {
    for (const auto& go : m_SceneContext.GameObjects) {
        go->LateUpdate(m_SceneContext);
    }
}

void AScene::FixedUpdate() {
    for (const auto& go : m_SceneContext.GameObjects) {
        go->FixedUpdate(m_SceneContext);
    }
}

void AScene::Destroyed() {
    for (const auto& go : m_SceneContext.GameObjects) {
        go->Destroyed(m_SceneContext);
    }
}

void AScene::DrawMainPass() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Graphics::GetWindowSize().at(0), Graphics::GetWindowSize().at(1));
    glClear(GL_COLOR_BUFFER_BIT);

    for (const auto& go : m_SceneContext.GameObjects) {
        const auto drawable = go->Cast<IDrawable>();
        if (drawable) {
            drawable->Draw(m_SceneContext);
        }
    }
}

void AScene::Render() {
    DrawMainPass();
}

IGameObject* AScene::FindGameObject(FSceneContext& context, const std::string& name) {
    for (const auto& go : context.GameObjects) {
        if (go->GetName() == name) {
            return go.get();
        }
    }

    return nullptr;
}