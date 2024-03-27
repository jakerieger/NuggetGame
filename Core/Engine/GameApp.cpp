//
// Created by jr on 2/12/24.
//

#include "GameApp.h"

#include "Color.h"
#include "DebugUI.h"
#include "GraphicsContext.h"
#include "GraphicsError.h"
#include "PhysicsContext.h"
#include "Profiler.h"

#include <thread>

namespace Application {
    static constexpr float FIXED_TIMESTEP = 1.f / 60.f;
    AColor g_ClearColor(0xFF9eb9df);
    // =========================================================
    bool IsRunning() { return !glfwWindowShouldClose(Graphics::GetWindow()); }

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

#ifndef NDEBUG
        Profiler::Initialize();
        DebugUI::Initialize();
        Graphics::Error::EnableDebugOutput();
        Profiler::Start();
#endif

        app.Startup();
    }

    void Update(IGameApp& app) {
        Graphics::ResetDrawCalls();
        Graphics::UpdateFrameTime();

        Physics::Tick(FIXED_TIMESTEP);

        const float frameTime  = Graphics::GetDeltaTime();
        const auto activeScene = app.GetActiveScene();

        // Update scene
        if (activeScene) {
            activeScene->Update(frameTime);
        }

// Update engine analytics
#ifndef NDEBUG
        Profiler::Update();
        DebugUI::Update(frameTime, activeScene);
#endif

        // Clear buffers
        glClearColor(g_ClearColor.Red, g_ClearColor.Green, g_ClearColor.Blue, g_ClearColor.Alpha);
        // Enable transparency in rendering
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_SCISSOR_TEST);

        // Render scene + UI
        if (activeScene) {
            activeScene->Render();
        }

#ifndef NDEBUG
        DebugUI::Draw();
#endif

        // Swap buffers and poll events
        glfwSwapBuffers(Graphics::GetWindow());
        glfwPollEvents();

        // Execute user late update logic
        if (activeScene) {
            activeScene->LateUpdate();
        }
    }

    void FixedUpdate(IGameApp& app) {
        if (const auto activeScene = app.GetActiveScene())
            activeScene->FixedUpdate();
    }

    void RunApp(IGameApp& app) {
        std::thread fixedThread([&]() {
            while (IsRunning()) {
                FixedUpdate(app);
                // std::this_thread::sleep_for(std::chrono::duration<double>(FIXED_TIMESTEP));
            }
        });

        while (IsRunning()) {
            Update(app);
        }

        fixedThread.join();

        app.Cleanup();
#ifndef NDEBUG
        DebugUI::Shutdown();
        Profiler::Shutdown();
#endif
        Graphics::Shutdown();
        Physics::Shutdown();
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