#include "Floor.h"
#include "Engine/GameApp.h"
#include "Engine/InputCodes.h"
#include "Nugget.h"
#include "Packer.h"
#include "Unpacker.h"
#include "Utilities.inl"
#include "Engine/GraphicsContext.h"
#include "Engine/Resources.h"
#include "GameObject.h"
#include "Level.h"
#include "Levels.h"
#include "Logger.h"
#include "Engine/AudioContext.h"
#include "Engine/GameUI.h"
#include "UI/PauseMenu.h"

#include <filesystem>
#include <fmt/format.h>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

class NuggetGame final : public IGameApp {
public:
    NuggetGame() = default;
    void Startup() override;
    void Cleanup() override;
    void OnKeyDown(FKeyEvent& event) override;

private:
    Rml::ElementDocument* m_PauseMenu = nullptr;
};

void NuggetGame::Startup() {
    auto testScene = Scene::Create("Test");
    auto nugget    = GameObject::Create<Nugget>("nugget");
    auto floor     = GameObject::Create<Floor>("floor");

    testScene->AddGameObject<Nugget>(nugget);
    testScene->AddGameObject<Floor>(floor);

    AddScene(testScene);
    LoadScene("Test");

    const auto bgMusicPath =
      Utilities::JoinPath(Resources::GetRoot(), "Assets", "audio", "gameplay.wav");
    Audio::PlayLoop(bgMusicPath.string(), 0.33f);

    Level testLevel("test", Levels::LvlTest);
}

void NuggetGame::Cleanup() {}

void NuggetGame::OnKeyDown(FKeyEvent& event) {
    IInputListener::OnKeyDown(event);

    if (event.KeyCode == KeyCode::Escape) {
        if (Paused()) {
            SetPaused(false);
            if (m_PauseMenu) {
                UI::CloseDocument(m_PauseMenu);
            }
        } else {
            SetPaused(true);
            m_PauseMenu = UI::CreateDocument(PauseMenu::Code);
            if (m_PauseMenu) {
                m_PauseMenu->Show();
            }
        }
        const auto windowTitle = fmt::format("Nugget Game {}", Paused() ? "(Paused)" : "");
        glfwSetWindowTitle(Graphics::GetWindow(), windowTitle.c_str());
    } else if (event.KeyCode == KeyCode::Tab) {
        Graphics::ToggleWireframe();
    } else if (event.KeyCode == KeyCode::F11) {
        Graphics::ToggleFullscreen();
    } else if (event.KeyCode == KeyCode::R) {
        // LoadScene("Test");
    }
}

int main(int argc, char* argv[]) {
    Resources::SetCwd(argv[0]);
    using namespace Packer::Schemas;

    // Unpack game assets
    {
        const auto dataRoot = Resources::GetRoot() / "Data";
        Unpacker::UnpackSprites(dataRoot, Resources::GetSprites());

        Logger::LogInfo(Logger::Subsystems::RUNTIME, "Unpacked game asset");
    }

    // Run game
    {
        NuggetGame app;
        Application::InitializeApp(app, 1280, 720, "Nugget Game", false);
        Utilities::SetWindowIcon(
          Resources::GetResource<Sprite>(Resources::ResourceType::Sprite, "nugget.png"));

#ifdef _WIN32
        ::FreeConsole();
#endif

        Application::RunApp(app);
    }

    // Dump logs for debugging
    Logger::DumpToDisk();

    return 0;
}