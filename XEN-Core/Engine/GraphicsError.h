//
// Created by jr on 2/13/24.
//

#pragma once

#include <glad/glad.h>

/**
 * \brief Error handling for OpenGL
 */
namespace Graphics::Error {
    void APIENTRY GLDebugCallback(GLenum source,
                                  GLenum type,
                                  unsigned int id,
                                  GLenum severity,
                                  GLsizei length,
                                  const char* message,
                                  const void* userParam);
    void EnableDebugOutput();
}  // namespace Graphics::Error
