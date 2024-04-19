#include "Engine/GameApp.h"
#include "Engine/InputCodes.h"
#include "Nugget.h"
#include "Packer.h"
#include "Unpacker.h"
#include "Utilities.inl"
#include "Engine/GraphicsContext.h"
#include "Engine/Resources.h"
#include "GameObject.h"
#include "Logger.h"
#include "Engine/AudioContext.h"
#include "Engine/EngineSettings.h"
#include "UI/MainMenu.h"
#include "Levels.h"

#include <filesystem>
#include <UI/SettingsMenu.h>
#include <fmt/format.h>

class NuggetGame final : public IGameApp {
public:
    NuggetGame() = default;
    void Startup() override;
    void Cleanup() override;
    void OnKeyDown(FKeyEvent& event) override;
};

void NuggetGame::Startup() {
    IGameApp::Startup();

    auto testScene = Scene::Create("Test");
    auto nugget    = GameObject::Create<Nugget>("nugget");
    auto level     = std::make_unique<Level>("Test", Levels::LvlTest);
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

    const auto bgMusicPath =
      Utilities::JoinPath(Resources::GetRoot(), "Assets", "audio", "gameplay.wav");
    Audio::PlayLoop(bgMusicPath.string(), Audio::EAudioTag::Music, 0.1f);
}

void NuggetGame::Cleanup() {}

void NuggetGame::OnKeyDown(FKeyEvent& event) {
    IInputListener::OnKeyDown(event);

    if (event.KeyCode == KeyCode::Escape) {
        const auto windowTitle = fmt::format("Nugget Game {}", Paused() ? "(Paused)" : "");
        glfwSetWindowTitle(Graphics::GetWindow(), windowTitle.c_str());
    } else if (event.KeyCode == KeyCode::Tab) {
        Graphics::ToggleWireframe();
    } else if (event.KeyCode == KeyCode::F11) {
        Graphics::ToggleFullscreen();
    } else if (event.KeyCode == KeyCode::R) {
        LoadScene("MainMenu");
    }
}

int main(int argc, char* argv[]) {
    Resources::SetCwd(argv[0]);
    using namespace Packer::Schemas;

    // Check if config file exists and create it if not
    if (!exists(Resources::GetConfigFile())) {
        Settings::SaveSettings();
    }

    // Unpack game assets
    {
        const auto dataRoot = Resources::GetRoot() / "Data";
        Unpacker::UnpackSprites(dataRoot, Resources::GetSprites());

        Logger::LogInfo(Logger::Subsystems::RUNTIME, "Unpacked game asset");
    }

    // Run game
    {
        NuggetGame app;
        Application::InitializeApp(app, "Nugget Game");
        Utilities::SetWindowIcon(Resources::GetRoot() / "icon.png");

#ifdef _WIN32
        // ::FreeConsole();
#endif

        Application::RunApp(app);
    }

    // Dump logs for debugging
    Logger::DumpToDisk();

    return 0;
}