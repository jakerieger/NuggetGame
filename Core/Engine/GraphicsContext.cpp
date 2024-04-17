//
// Created by jr on 2/12/24.
//

#include "GraphicsContext.h"

#include "EngineSettings.h"
#include "GraphicsError.h"
#include "Logger.h"

#ifdef _WIN32
    #define GLFW_EXPOSE_NATIVE_WIN32
#elif __APPLE__
    #define GLFW_EXPOSE_NATIVE_COCOA
#elif __linux__
    #define GLFW_EXPOSE_NATIVE_X11
#endif
#include "AudioContext.h"

#include <GLFW/glfw3native.h>

namespace Graphics {
    // Window vars
    GL::TWindow g_Window;
    i32 g_InitWindowWidth;
    i32 g_InitWindowHeight;
    i32 g_CurrentWindowWidth;
    i32 g_CurrentWindowHeight;
    const char* g_WindowTitle;
    float g_DeltaTime = 0.f;
    u32 g_DrawCalls   = 0;

    // Graphics state
    float g_LastFrame   = 0.f;
    bool g_IsWireframe  = false;
    bool g_IsFullscreen = false;
    bool g_EnableVsync  = false;

    GLFWwindow* GetWindow() {
        return g_Window.get();
    }

#ifdef _WIN32
    HWND GetNativeWindow() {
        return glfwGetWin32Window(g_Window.get());
    }
#elif __linux__
    Window GetNativeWindow() {
        return glfwGetX11Window(g_Window.get());
    }
#elif __APPLE__
    NSWindow GetNativeWindow() {
        return glfwGetCocoaWindow(g_Window.get());
    }
#endif

    void UpdateFrameTime() {
        const auto currentFrame = static_cast<float>(glfwGetTime());
        g_DeltaTime             = currentFrame - g_LastFrame;
        g_LastFrame             = currentFrame;
    }

    float GetDeltaTime() {
        return g_DeltaTime;
    }

    void AddDrawCall() {
        g_DrawCalls++;
    }

    void ResetDrawCalls() {
        g_DrawCalls = 0;
    }

    u32 GetDrawCalls() {
        return g_DrawCalls;
    }

    float GetFrameRate() {
        return 1.f / GetDeltaTime();
    }

    void ToggleWireframe() {
        if (g_IsWireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        g_IsWireframe = !g_IsWireframe;
    }

    void ToggleFullscreen() {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        if (g_IsFullscreen) {
            glfwSetWindowMonitor(GetWindow(),
                                 nullptr,
                                 0,
                                 0,
                                 g_InitWindowWidth,
                                 g_InitWindowHeight,
                                 mode->refreshRate);

            g_CurrentWindowWidth  = g_InitWindowWidth;
            g_CurrentWindowHeight = g_InitWindowHeight;
        } else {
            glfwSetWindowMonitor(GetWindow(),
                                 glfwGetPrimaryMonitor(),
                                 0,
                                 0,
                                 mode->width,
                                 mode->height,
                                 mode->refreshRate);

            g_CurrentWindowWidth  = mode->width;
            g_CurrentWindowHeight = mode->height;
        }

        g_IsFullscreen = !g_IsFullscreen;
    }

    void ToggleVsync() {
        if (g_EnableVsync) {
            glfwSwapInterval(0);
        } else {
            glfwSwapInterval(1);
        }

        g_EnableVsync = !g_EnableVsync;
    }

    void MarkWindowForClose() {
        glfwSetWindowShouldClose(GetWindow(), true);
    }

    std::array<int32_t, 2> GetWindowSize() {
        return {
          g_CurrentWindowWidth,
          g_CurrentWindowHeight,
        };
    }

    glm::vec2 GetWindowSizeGLM() {
        return {
          g_CurrentWindowWidth,
          g_CurrentWindowHeight,
        };
    }

    ImVec2 GetWindowSizeImGui() {
        return {
          static_cast<f32>(g_CurrentWindowWidth),
          static_cast<f32>(g_CurrentWindowHeight),
        };
    }

    float GetWindowAspect() {
        return (static_cast<float>(g_CurrentWindowWidth) /
                static_cast<float>(g_CurrentWindowHeight));
    }

    bool Initialize(const char* title) {
        const auto width   = static_cast<i32>(Settings::GetSettings().Graphics.ResolutionX);
        const auto height  = static_cast<i32>(Settings::GetSettings().Graphics.ResolutionY);
        g_InitWindowWidth  = width;
        g_InitWindowHeight = height;
        g_WindowTitle      = title;

        if (glfwInit() != GLFW_TRUE) {
            return false;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);

#ifndef NDEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

        g_Window = GL::TWindow(glfwCreateWindow(width, height, g_WindowTitle, nullptr, nullptr));
        if (g_Window == nullptr) {
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(g_Window.get());

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            return false;
        }

        GL::FramebufferCallback(g_Window.get(), width, height);
        glfwSetFramebufferSizeCallback(GetWindow(), GL::FramebufferCallback);

        glfwSwapInterval(Settings::GetSettings().Graphics.Vsync ? 1 : 0);

        // glfwSetInputMode(g_Window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        Logger::LogInfo(Logger::Subsystems::GRAPHICS, "Graphics subsystem initialized.");

        return true;
    }

    void Shutdown() {
        glfwDestroyWindow(GetWindow());
        glfwTerminate();
    }

    namespace GL {
        void UpdateWindowSize(const int width, const int height) {
            g_CurrentWindowWidth  = width;
            g_CurrentWindowHeight = height;
        }
    }  // namespace GL
}  // namespace Graphics