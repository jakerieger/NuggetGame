//
// Created by jr on 2/12/24.
//

#pragma once

#include "STL.h"
#include "NativePlatform.h"

#include <array>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <glm/vec2.hpp>

namespace Graphics {
    GLFWwindow* GetWindow();
    void ToggleWireframe();
    void ToggleFullscreen();
    void ToggleVsync();
    void MarkWindowForClose();
    std::array<int32_t, 2> GetWindowSize();
    float GetWindowAspect();
    void UpdateFrameTime();
    float GetFrameRate();
    float GetDeltaTime();
    void AddDrawCall();
    u32 GetDrawCalls();
    void ResetDrawCalls();
    bool Initialize(const char* title);
    void Shutdown();

#ifdef _WIN32
    HWND GetNativeWindow();
#elif __linux__
    Window GetNativeWindow();
#elif __APPLE__
    NSWindow GetNativeWindow();
#endif

    glm::vec2 GetWindowSizeGLM();
    ImVec2 GetWindowSizeImGui();

    namespace GL {
        void UpdateWindowSize(int width, int height);

        inline void FramebufferCallback(GLFWwindow* window, int width, int height) {
            UpdateWindowSize(width, height);
            glViewport(0, 0, width, height);
        }

        struct FDestroyWindow {
            void operator()(GLFWwindow* ptr) const noexcept {
                glfwDestroyWindow(ptr);
            }
        };

        using TWindow = std::unique_ptr<GLFWwindow, FDestroyWindow>;
    }  // namespace GL
}  // namespace Graphics