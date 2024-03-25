//
// Created by conta on 2/16/2024.
//

#include "Transform.h"

ATransform::ATransform() {
    m_ModelMatrix = glm::mat4(1.f);
    m_Position    = glm::vec2(0.f);
    m_Rotation    = glm::vec2(0.f);
    m_Scale       = glm::vec2(1.f);
}

void ATransform::Translate(const glm::vec2& translation) { m_Position += translation; }

void ATransform::Translate(const float x, const float y) {
    const glm::vec2 trans = {x, y};
    Translate(trans);
}

void ATransform::Rotate(const float angle, const EAxis axis) {
    switch (axis) {
        case EAxis::X:
            m_Rotation.x += angle;
            break;
        case EAxis::Y:
            m_Rotation.y += angle;
            break;
    }
}

void ATransform::Scale(const glm::vec2& scale) { m_Scale *= scale; }

void ATransform::Scale(const float x, const float y) {
    const glm::vec2 scale = {x, y};
    Scale(scale);
}

void ATransform::SetPositionAndRotation(const glm::vec2& position, const glm::vec2& rotation) {
    m_Position = position;
    m_Rotation = rotation;
}

void ATransform::SetPositionAndRotation(const float posX,
                                        const float posY,
                                        const float rotX,
                                        const float rotY) {
    const glm::vec2 pos = {posX, posY};
    const glm::vec2 rot = {rotX, rotY};
    SetPositionAndRotation(pos, rot);
}

void ATransform::SetPosition(const glm::vec2& pos) { m_Position = pos; }

void ATransform::SetPosition(float x, float y) { m_Position = {x, y}; }

void ATransform::SetRotation(const glm::vec2& rot) { m_Rotation = rot; }

void ATransform::SetRotation(float x, float y) { m_Rotation = {x, y}; }

void ATransform::SetScale(const glm::vec2& scale) { m_Scale = scale; }

void ATransform::SetScale(float x, float y) { m_Scale = {x, y}; }

void ATransform::Update(float deltaTime, FSceneContext& sceneContext) {
    // m_ModelMatrix = glm::mat4(1.0f);
    // m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
    // m_ModelMatrix =
    //   glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    // m_ModelMatrix =
    //   glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    // m_ModelMatrix =
    //   glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    // m_ModelMatrix = glm::scale(m_ModelMatrix, m_Scale);
}