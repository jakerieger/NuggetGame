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

#define RUNNING IsRunning()

namespace Application {
    static constexpr float FIXED_TIMESTEP = 1.f / 60.f;
    static constexpr i64 SLEEP_FOR        = 2;
    AColor g_ClearColor(0xFF9eb9df);
    // =========================================================
    bool IsRunning() { return !glfwWindowShouldClose(Graphics::GetWindow()); }

    //
    // (1/60)   1/1000
    // ------ = ------
    //    2        x
    float GetSleepDuration(float timestep) {
        auto t1 = (1.f / 60.f);
        auto s1 = 2.f;
        return (s1 * timestep) / t1;
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

#ifndef NDEBUG
        Profiler::Initialize();
        Debug::UI::Initialize();
        Graphics::Error::EnableDebugOutput();
        Profiler::Start();
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
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_SCISSOR_TEST);

        // Render scene + UI
        if (activeScene) {
            activeScene->Render();
        }

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
        Physics::Tick(FIXED_TIMESTEP);
        if (const auto activeScene = app.GetActiveScene())
            activeScene->FixedUpdate();
    }

    void RunApp(IGameApp& app) {
        std::thread fixedThread([&]() {
            while (RUNNING) {
                FixedUpdate(app);
                // I don't understand why this is the correct sleep duration,
                // but it works. The relationship is 0.01 : 2 for timestep : sleep
                // where sleep is in milliseconds.
                std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_FOR));
            }
        });

        while (RUNNING) {
            Update(app);
        }

        fixedThread.join();

        app.Cleanup();
#ifndef NDEBUG
        Debug::UI::Shutdown();
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