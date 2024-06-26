#include "Engine/GameApp.h"
#include "Engine/InputCodes.h"
#include "Nugget.h"
#include "Utilities.inl"
#include "Engine/GraphicsContext.h"
#include "Engine/Resources.h"
#include "GameObject.h"
#include "Logger.h"
#include "Engine/AudioContext.h"
#include "Engine/EngineSettings.h"
#include "UI/MainMenu.h"
#include "Levels.h"
#include "XPak.h"
#include "PlatformTools.h"

#include <filesystem>
#include <iostream>
#include <UI/SettingsMenu.h>
#include <fmt/format.h>

class NuggetGame final : public IGameApp {
public:
    NuggetGame() = default;
    void Startup() override;
    void Cleanup() override;
    void OnKeyDown(FKeyEvent& event) override;

private:
    FLevel m_CurrentLevel = Levels::LvlUpAndUp;
};

void NuggetGame::Startup() {
    IGameApp::Startup();

    auto testScene = Scene::Create("Test");
    auto nugget    = std::make_unique<Nugget>("Player", m_CurrentLevel.PlayerStart);
    auto level     = std::make_unique<Level>("UpAndUp", m_CurrentLevel);
    testScene->AddGameObject(nugget);
    testScene->AddGameObject(level);
    AddScene(testScene);

    auto mainMenuScene = Scene::Create("MainMenu");
    auto mainMenu      = GameObject::Create<MainMenu>("MainMenu");
    mainMenuScene->AddGameObject(mainMenu);
    AddScene(mainMenuScene);

    auto settingsScene = Scene::Create("Settings");
    auto settingsMenu  = GameObject::Create<SettingsMenu>("SettingsMenu");
    settingsScene->AddGameObject(settingsMenu);
    AddScene(settingsScene);

    LoadScene("MainMenu");

    // Audio::PlayLoop(bgMusicPath.string(), "BackgroundMusic", Audio::EAudioTag::Music, 0.6f);

    const auto bgMusic = Resources::GetAsset("Assets/audio/gameplay.wav");
    Audio::PlayLoop(bgMusic, "BackgroundMusic", Audio::EAudioTag::Music, 0.6f);
}

void NuggetGame::Cleanup() {}

void NuggetGame::OnKeyDown(FKeyEvent& event) {
    IInputListener::OnKeyDown(event);

    if (event.KeyCode == KeyCode::Escape) {
        const auto windowTitle = fmt::format("Nugget Game {}", Paused() ? "(Paused)" : "");
        glfwSetWindowTitle(Graphics::GetWindow(), windowTitle.c_str());
        TogglePause();
    } else if (event.KeyCode == KeyCode::Tab) {
        Graphics::ToggleWireframe();
    } else if (event.KeyCode == KeyCode::F11) {
        Graphics::ToggleFullscreen();
    } else if (event.KeyCode == KeyCode::R) {
        const auto sceneName = GetActiveScene()->GetName();
        LoadScene(sceneName);
    }
}

int main(int argc, char* argv[]) {
    Resources::SetCwd(argv[0]);

    // Check if config file exists and create it if not
    if (!exists(Resources::GetConfigFile())) {
        Settings::SaveSettings();
    }

    const auto dataRoot = Resources::GetRoot() / "Data";
    if (!exists(dataRoot)) {
        Resources::UsingPak = false;
    }

    // Unpack game assets
    if (Resources::UsingPak) {
        auto assets = XPak::UnpackAll(dataRoot);
        assert(!assets.empty());
        Resources::Assets.insert(Resources::Assets.end(), assets.begin(), assets.end());
    }

    NuggetGame app;
    Application::InitializeApp(app, "Nugget Game");
    Utilities::SetWindowIcon(Resources::GetRoot() / "icon.png");

    #ifdef _WIN32
    // ::FreeConsole();
    #endif

    Application::RunApp(app);

    // Dump logs for debugging
    Logger::DumpToDisk();

    return 0;
}