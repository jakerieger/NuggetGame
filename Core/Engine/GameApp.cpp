//
// Created by jr on 2/12/24.
//

#include "GameApp.h"

#include "Color.h"
#include "GraphicsContext.h"
#include "GraphicsError.h"
#include "PhysicsContext.h"

namespace Application {
    AColor g_ClearColor(0xFF9eb9df);
    // =========================================================

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
        Graphics::Error::EnableDebugOutput();
#endif

        app.Startup();
    }

    bool UpdateApp(IGameApp& app) {
        Graphics::UpdateFrameTime();

        // FixedUpdate time step is a constant 1000 FPS
        // Used for polling IO events and physics simulations
        constexpr float FIXED_TIME_STEP = 1.0 / 1000.0;
        static float accumulatedTime    = 0.0;
        const float frameTime           = Graphics::GetDeltaTime();
        accumulatedTime += frameTime;

        const auto activeScene = app.GetActiveScene();

        // Fixed update loop
        while (accumulatedTime >= FIXED_TIME_STEP) {
            if (activeScene) {
                activeScene->FixedUpdated();
            }

            accumulatedTime -= FIXED_TIME_STEP;
        }

        // Tick physics simulation
        Physics::Tick(FIXED_TIME_STEP);

        // Update scene
        if (activeScene) {
            activeScene->Update(frameTime);
        }

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

        // Swap buffers and poll events
        glfwSwapBuffers(Graphics::GetWindow());
        glfwPollEvents();

        // Execute user late update logic
        if (activeScene) {
            activeScene->LateUpdate();
        }

        return !glfwWindowShouldClose(Graphics::GetWindow());
    }

    void RunApp(IGameApp& app) {
        while (UpdateApp(app)) {}

        app.Cleanup();
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