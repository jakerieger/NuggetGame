//
// Created by jr on 2/13/24.
//

#include "GraphicsError.h"

#include "Logger.h"

#include <cstdio>

namespace Graphics::Error {
    void EnableDebugOutput() {
        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            Logger::LogInfo(Logger::Subsystems::GRAPHICS, "Graphics debugging is enabled");
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(GLDebugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        } else {
            Logger::LogError(Logger::Subsystems::GRAPHICS,
                             "Graphics debugging failed to initialize");
        }
    }

    void APIENTRY GLDebugCallback(GLenum source,
                                  GLenum type,
                                  unsigned int id,
                                  GLenum severity,
                                  GLsizei length,
                                  const char* message,
                                  const void* userParam) {
        // ignore non-significant error/warning codes
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
            return;

        printf("---------------\n");
        printf("Debug message (%d): %s\n", id, message);

        switch (source) {
            case GL_DEBUG_SOURCE_API:
                printf("Source: API");
                break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                printf("Source: Window System");
                break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                printf("Source: Shader Compiler");
                break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:
                printf("Source: Third Party");
                break;
            case GL_DEBUG_SOURCE_APPLICATION:
                printf("Source: Application");
                break;
            case GL_DEBUG_SOURCE_OTHER:
                printf("Source: Other");
                break;
        }
        printf("\n");

        switch (type) {
            case GL_DEBUG_TYPE_ERROR:
                printf("Type: Error");
                break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                printf("Type: Deprecated Behaviour");
                break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                printf("Type: Undefined Behaviour");
                break;
            case GL_DEBUG_TYPE_PORTABILITY:
                printf("Type: Portability");
                break;
            case GL_DEBUG_TYPE_PERFORMANCE:
                printf("Type: Performance");
                break;
            case GL_DEBUG_TYPE_MARKER:
                printf("Type: Marker");
                break;
            case GL_DEBUG_TYPE_PUSH_GROUP:
                printf("Type: Push Group");
                break;
            case GL_DEBUG_TYPE_POP_GROUP:
                printf("Type: Pop Group");
                break;
            case GL_DEBUG_TYPE_OTHER:
                printf("Type: Other");
                break;
        }
        printf("\n");

        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:
                printf("Severity: high");
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                printf("Severity: medium");
                break;
            case GL_DEBUG_SEVERITY_LOW:
                printf("Severity: low");
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                printf("Severity: notification");
                break;
        }
        printf("\n\n");
    }
}  // namespace Graphics::Error