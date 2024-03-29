//
// Created by jr on 2/12/24.
//

#include "GameApp.h"
#include "GraphicsContext.h"
#include "Color.h"
#include "DebugUI.h"
#include "GameUI.h"
#include "GraphicsError.h"
#include "PhysicsContext.h"
#include "Profiler.h"

#define RUNNING Application::IsRunning()

namespace Application {
    static constexpr float FIXED_TIMESTEP = 1.f / 60.f;
    static constexpr float ADJUSTMENT     = 8.f;
    AColor g_ClearColor(0xFF9eb9df);
    // =========================================================
    bool IsRunning() { return !glfwWindowShouldClose(Graphics::GetWindow()); }

    static void Shutdown() {
#ifndef NDEBUG
        Profiler::Shutdown();
        Debug::UI::Shutdown();
#endif
        UI::Shutdown();
        Graphics::Shutdown();
        Physics::Shutdown();
    }

    void InitializeApp(IGameApp& app,
                       const int width,
                       const int height,
                       const char* title,
                       const bool launchFullscreen) {
        Graphics::Initialize(width, height, title);
        if (launchFullscreen) {
            Graphics::ToggleFullscreen();
        }
        Physics::Init();
        Input::Initialize(Graphics::GetWindow());
        UI::Initialize();

#ifndef NDEBUG
        Profiler::Initialize();
        Graphics::Error::EnableDebugOutput();
        Profiler::Start();
        Debug::UI::Initialize();
#endif

        app.Startup();
    }

    void Update(IGameApp& app) {
        Graphics::ResetDrawCalls();
        Graphics::UpdateFrameTime();

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
        Debug::UI::Update(frameTime, activeScene);
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
        Debug::UI::Draw();
#endif

        // Swap buffers and poll events
        glfwSwapBuffers(Graphics::GetWindow());
        glfwPollEvents();

        // Execute user late update logic
        if (activeScene) {
            activeScene->LateUpdate();
        }
    }

    static void FixedUpdate(IGameApp& app) {
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
                FixedUpdate(app);
                accumulator -= dt;
                t += dt;
            }

            Update(app);
        }

        app.Cleanup();

        Shutdown();
    }
}  // namespace Application

void IGameApp::AddScene(std::unique_ptr<AScene>& scene) { m_Scenes.push_back(std::move(scene)); }

void IGameApp::LoadScene(const std::string& name) {
    const auto scene = GetScene(name);
    if (const auto currentScene = GetActiveScene()) {
        UnloadScene(currentScene);
    }

    if (scene) {
        scene->SetActive(true);
        scene->Start();

        for (auto& go : scene->GetContext().GameObjects) {
            Input::RegisterListener(go.get());
        }
    }
}

void IGameApp::UnloadScene(AScene* scene) {
    Input::UnregisterSceneListeners(this);

    // TODO: Intead of initializing gameobjects in their constructor,
    // move the logic to the Awake method so that gameobjects
    // can be reinitialized on scene load and not permanently
    // destroyed when unloading a scene
    // scene->Destroyed();
    scene->SetActive(false);
}

void IGameApp::SetPaused(const bool paused) { m_Paused = paused; }