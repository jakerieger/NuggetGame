//
// Created by jr on 2/12/24.
//

#pragma once
#include "STL.h"
#include "Scene.h"
#include "Input.h"

class IGameApp : public IInputListener {
public:
    IGameApp() { Input::RegisterListener(this); }
    virtual void Startup() = 0;
    virtual void Cleanup() = 0;

    void AddScene(std::unique_ptr<AScene>& scene);
    void LoadScene(const std::string& name);
    void UnloadScene(AScene* scene);
    void SetPaused(bool paused);
    bool Paused() const { return m_Paused; }

    AScene* GetActiveScene() {
        for (const auto& scene : m_Scenes) {
            if (scene->GetActive()) {
                return scene.get();
            }
        }
        return nullptr;
    }

    AScene* GetScene(const std::string& name) {
        for (const auto& scene : m_Scenes) {
            if (scene->GetName() == name) {
                return scene.get();
            }
        }
        return nullptr;
    }

protected:
    std::vector<std::unique_ptr<AScene>> m_Scenes;

private:
    bool m_Paused = false;
};

namespace Application {
    void
    InitializeApp(IGameApp& app, int width, int height, const char* title, bool launchFullscreen);
    void RunApp(IGameApp& app);
    bool IsRunning();
}  // namespace Application

#define CREATE_AND_RUN(appClass, size, fullscreen)                                                 \
    int main(int argc, char* argv[]) {                                                             \
        Resources::SetCwd(argv[0]);                                                                \
        appClass app;                                                                              \
        Application::InitializeApp(app, size, #appClass, fullscreen);                              \
        Utilities::SetWindowIcon(Resources::GetResource(RES_ROOT, "APP_ICON.png").c_str());        \
        Application::RunApp(app);                                                                  \
        return 0;                                                                                  \
    }