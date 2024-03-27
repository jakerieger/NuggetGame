#include "Floor.h"
#include "Engine/GameApp.h"
#include "InputCodes.h"
#include "Nugget.h"
#include "Packer.h"
#include "Unpacker.h"
#include "Utilities.inl"
#include "Engine/GraphicsContext.h"
#include "Engine/Resources.h"

#include <filesystem>

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
};

void NuggetGame::Startup() {
    auto testScene = AScene::Create("Test");
    auto nugget    = IGameObject::Create<Nugget>("nugget");

    testScene->AddGameObject<Nugget>(nugget);

    static constexpr u32 NUM_FLOOR_TILES = 640 / 32;
    for (auto i = 0; i < NUM_FLOOR_TILES; ++i) {
        auto floorTile =
          // ReSharper disable once CppSmartPointerVsMakeFunction
          std::unique_ptr<Floor>(new Floor(std::to_string(i), {(32.f * i) + 16.f, 16.f}));
        testScene->AddGameObject<Floor>(floorTile);
    }

    AddScene(testScene);
    LoadScene("Test");
}

void NuggetGame::Cleanup() {}

void NuggetGame::OnKeyDown(FKeyEvent& event) {
    IInputListener::OnKeyDown(event);

    if (event.KeyCode == KeyCode::Escape) {
        Graphics::MarkWindowForClose();
    } else if (event.KeyCode == KeyCode::Tab) {
        Graphics::ToggleWireframe();
    }
}

int main(int argc, char* argv[]) {
    Resources::SetCwd(argv[0]);
    using namespace Packer::Schemas;

    // Unpack game assets
    {
        const auto dataRoot = Resources::GetRoot() / "Data";
        Unpacker::UnpackSprites(dataRoot, Resources::GetSprites());

        std::cout << "Unpacked game assets\n";
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

    return 0;
}