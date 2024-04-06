//
// Created by conta on 2/26/2024.
//

#pragma once

#include "Primitives.h"
#include "Shader.h"
#include "STL.h"
#include "Engine/GraphicsContext.h"

#include <glad/glad.h>
#include <memory>

class ASpriteBatch {
public:
    ASpriteBatch() = default;
    void Init(const std::vector<glm::mat4>& modelMatrices);
    void UpdateInstances(const std::vector<glm::mat4>& modelMatrices) const;
    void Draw(u32 texture, const glm::mat4& view, const glm::mat4& projection, u32 count) const;
    void Cleanup() const;

private:
    u32 m_VBO         = 0;
    u32 m_VAO         = 0;
    u32 m_InstanceVBO = 0;
    std::unique_ptr<AShader> m_Shader;
};

inline void ASpriteBatch::Init(const std::vector<glm::mat4>& modelMatrices) {
    m_Shader                   = std::make_unique<AShader>(BuiltinShaders::SpriteBatch);
    const size_t instanceCount = modelMatrices.size();

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_InstanceVBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Primitives::Quad), Primitives::Quad, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(glm::mat4) * instanceCount,
                 modelMatrices.data(),
                 GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(glm::mat4),
                          (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(glm::mat4),
                          (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);

    m_Shader->Use();
    m_Shader->SetInt("u_Sprite", 0);
}

inline void ASpriteBatch::UpdateInstances(const std::vector<glm::mat4>& modelMatrices) const {
    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    sizeof(glm::mat4) * modelMatrices.size(),
                    modelMatrices.data());
}

inline void ASpriteBatch::Draw(const u32 texture,
                               const glm::mat4& view,
                               const glm::mat4& projection,
                               u32 count) const {
    m_Shader->Use();
    m_Shader->SetMat4("u_View", view);
    m_Shader->SetMat4("u_Projection", projection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(m_VAO);
    Graphics::AddDrawCall();
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);
    glBindVertexArray(0);
}

inline void ASpriteBatch::Cleanup() const {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}
