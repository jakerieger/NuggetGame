//
// Created by conta on 2/16/2024.
//

#include "Transform.h"

ATransform::ATransform() {
    m_ModelMatrix = glm::mat4(1.f);
    m_Position    = glm::vec2(0.f);
    m_Rotation    = 0.f;
    m_Scale       = glm::vec2(1.f);
}

void ATransform::Translate(const glm::vec2& translation) { m_Position += translation; }

void ATransform::Translate(const float x, const float y) {
    const glm::vec2 trans = {x, y};
    Translate(trans);
}

void ATransform::Rotate(const float angle) { m_Rotation += angle; }

void ATransform::Scale(const glm::vec2& scale) { m_Scale = scale; }

void ATransform::Scale(const float x, const float y) {
    const glm::vec2 scale = {x, y};
    Scale(scale);
}

void ATransform::SetPositionAndRotation(const glm::vec2& position, float rotDegrees) {
    m_Position = position;
    m_Rotation = rotDegrees;
}

void ATransform::SetPositionAndRotation(const float posX, const float posY, float rotDegrees) {
    const glm::vec2 pos = {posX, posY};
    SetPositionAndRotation(pos, rotDegrees);
}

void ATransform::SetPosition(const glm::vec2& pos) { m_Position = pos; }

void ATransform::SetPosition(float x, float y) { m_Position = {x, y}; }

void ATransform::SetRotation(float deg) { m_Rotation = deg; }

void ATransform::SetScale(const glm::vec2& scale) { m_Scale = scale; }

void ATransform::SetScale(float x, float y) { m_Scale = {x, y}; }

void ATransform::Update(float deltaTime, FSceneContext& sceneContext) {
    m_ModelMatrix = glm::mat4(1.0f);
    m_ModelMatrix = translate(m_ModelMatrix, {m_Position, 0.f});
    m_ModelMatrix = rotate(m_ModelMatrix, glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    m_ModelMatrix = scale(m_ModelMatrix, {m_Scale, 0.f});
}