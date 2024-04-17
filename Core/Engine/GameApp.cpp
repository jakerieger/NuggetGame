//
// Created by jr on 2/12/24.
//

#include "GameApp.h"
#include "AudioContext.h"
#include "Camera.h"
#include "GraphicsContext.h"
#include "Color.h"
#include "EngineSettings.h"
#include "GameUI.h"
#include "GraphicsError.h"
#include "PhysicsContext.h"
#include "Profiler.h"
#include "Scene.h"

#include <Logger.h>

#define RUNNING Application::IsRunning()

namespace Application {
    static constexpr float FIXED_TIMESTEP = 1.f / 60.f;
    static constexpr float ADJUSTMENT     = 4.f;
    static IGameApp* g_CurrentApp         = nullptr;
    static bool g_IsLoading               = false;
    AColor g_ClearColor(0xFF9eb9df);

    bool IsRunning() {
        return !glfwWindowShouldClose(Graphics::GetWindow());
    }

    void SetLoading(const bool loading) {
        g_IsLoading = loading;
    }

    bool IsLoading() {
        return g_IsLoading;
    }

    static void Shutdown() {
        g_CurrentApp = nullptr;
#ifndef NDEBUG
        Profiler::Shutdown();
        // Debug::UI::Shutdown();
#endif
        Input::Shutdown();
        UI::Shutdown();
        Graphics::Shutdown();
        Physics::Shutdown();
        Audio::Shutdown();
    }

    void InitializeApp(IGameApp& app, const char* title) {
        g_CurrentApp = &app;

        if (!Settings::ReadSettings()) {
            return;
        }

        // Initialize engine subsytems
        {
            Graphics::Initialize(title);
            if (Settings::GetSettings().Fullscreen) {
                Graphics::ToggleFullscreen();
            }
            Camera::Initialize();
            Physics::Initialize();
            UI::Initialize();
            Audio::Initialize();
#ifndef NDEBUG
            Profiler::Initialize();
            Graphics::Error::EnableDebugOutput();
            Profiler::Start();
            // Debug::UI::Initialize();
#endif
            Input::Initialize(Graphics::GetWindow(), &app);
        }

        app.Startup();
    }

    IGameApp* GetCurrentApp() {
        return g_CurrentApp;
    }

    void Update(const IGameApp& app) {
        Graphics::ResetDrawCalls();
        Graphics::UpdateFrameTime();
        Camera::Update();

        const float frameTime  = Graphics::GetDeltaTime();
        const auto activeScene = app.GetActiveScene();

        // Update scene
        if (activeScene) {
            if (!app.Paused())
                activeScene->Update(frameTime);
        }

// Update engine analytics
#ifndef NDEBUG
        Profiler::Update();
        // Debug::UI::Update(frameTime, activeScene);
#endif

        // Clear buffers
        glClearColor(g_ClearColor.Red, g_ClearColor.Green, g_ClearColor.Blue, g_ClearColor.Alpha);
        // Enable transparency in rendering
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glEnable(GL_BLEND);
        glEnable(GL_MULTISAMPLE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_SCISSOR_TEST);

        // Render scene + UI
        if (activeScene) {
            activeScene->Render();
        }

        UI::Begin();
        UI::Draw();
        UI::End();

#ifndef NDEBUG
// Debug::UI::Draw();
#endif

        // Swap buffers and poll events
        glfwSwapBuffers(Graphics::GetWindow());
        glfwPollEvents();

        // Execute user late update logic
        if (activeScene) {
            activeScene->LateUpdate();
        }
    }

    static void FixedUpdate(const IGameApp& app) {
        if (!app.Paused()) {
            Physics::Tick(FIXED_TIMESTEP);
            if (const auto activeScene = app.GetActiveScene())
                activeScene->FixedUpdate();
        }
    }

    void RunApp(IGameApp& app) {
        double t            = 0.0;
        constexpr double dt = FIXED_TIMESTEP / ADJUSTMENT;
        double currentTime  = glfwGetTime();
        double accumulator  = 0.0;

        while (RUNNING) {
            const double newTime   = glfwGetTime();
            const double frameTime = newTime - currentTime;
            currentTime            = newTime;
            accumulator += frameTime;
            while (accumulator >= dt) {
                if (!g_IsLoading) {
                    FixedUpdate(app);
                }

                accumulator -= dt;
                t += dt;
            }

            if (!g_IsLoading) {
                Update(app);
            }
        }

        app.Cleanup();
        Shutdown();
    }
}  // namespace Application

void IGameApp::AddScene(std::unique_ptr<AScene>& scene) {
    m_Scenes.push_back(std::move(scene));
}

void IGameApp::LoadScene(const std::string& name) {
    Application::SetLoading(true);

    if (GetActiveScene()) {
        UnloadScene(GetActiveScene());
    }

    const auto scene = GetScene(name);
    if (scene) {
        scene->SetActive(true);
        scene->Start();
    } else {
        Logger::LogWarning(Logger::Subsystems::RUNTIME, "Scene '%s' does not exist.", name.c_str());
    }

    Application::SetLoading(false);
}

void IGameApp::UnloadScene(AScene* scene) {
    scene->Destroyed();
    scene->SetActive(false);
}

void IGameApp::SetPaused(const bool paused) {
    m_Paused = paused;
}