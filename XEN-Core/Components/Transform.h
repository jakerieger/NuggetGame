//
// Created by conta on 2/16/2024.
//

#pragma once

#include "Interfaces/Component.h"
#include "Types.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class EAxis : u8 { X, Y };

struct FSceneContext;

class ATransform final : public IComponent {
public:
    ATransform();

    [[nodiscard]] glm::mat4 GetModelMatrix() const { return m_ModelMatrix; }

    void Translate(const glm::vec2& translation);
    void Translate(float x, float y);
    void Rotate(float angle);
    void Scale(const glm::vec2& scale);
    void Scale(float x, float y);
    void SetPositionAndRotation(const glm::vec2& position, float rotDegrees);
    void SetPositionAndRotation(float posX, float posY, float rotDegrees);
    void SetPosition(const glm::vec2& pos);
    void SetPosition(float x, float y);
    void SetRotation(float deg);
    void SetScale(const glm::vec2& scale);
    void SetScale(float x, float y);

    void Update(float deltaTime, FSceneContext& sceneContext) override;

    [[nodiscard]] glm::vec2 GetPosition() const { return m_Position; }

    [[nodiscard]] float GetRotation() const { return m_Rotation; }

    [[nodiscard]] glm::vec2 GetScale() const { return m_Scale; }

private:
    glm::vec2 m_Position {};
    float m_Rotation {};
    glm::vec2 m_Scale {};
    glm::mat4 m_ModelMatrix {};
};