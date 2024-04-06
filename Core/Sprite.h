//
// Created by conta on 2/26/2024.
//

#pragma once

#include "Shader.h"
#include "STL.h"
#include "Primitives.h"
#include "Engine/GraphicsContext.h"

#include <glad/glad.h>
#include <memory>

class ASprite {
public:
    ASprite() = default;
    void Init();
    void Draw(u32 texture,
              const glm::mat4& view,
              const glm::mat4& projection,
              const glm::mat4& model) const;
    void Cleanup() const;

private:
    u32 m_VBO = 0;
    u32 m_VAO = 0;
    std::unique_ptr<AShader> m_Shader;
};

inline void ASprite::Init() {
    m_Shader = std::make_unique<AShader>(BuiltinShaders::Sprite);

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Primitives::Quad), Primitives::Quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    m_Shader->Use();
    m_Shader->SetInt("u_Sprite", 0);
}

inline void ASprite::Draw(const u32 texture,
                          const glm::mat4& view,
                          const glm::mat4& projection,
                          const glm::mat4& model) const {
    m_Shader->Use();
    m_Shader->SetMat4("u_Projection", projection);
    m_Shader->SetMat4("u_View", view);
    m_Shader->SetMat4("u_Model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(m_VAO);
    Graphics::AddDrawCall();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

inline void ASprite::Cleanup() const {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}
