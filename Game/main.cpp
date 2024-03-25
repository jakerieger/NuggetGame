#include "Engine/GameApp.h"
#include "InputCodes.h"
#include "Nugget.h"
#include "Utilities.inl"
#include "Engine/GraphicsContext.h"
#include "Engine/Resources.h"

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
    NuggetGame app;
    Application::InitializeApp(app, 1280, 720, "Nugget Game", false);
    Utilities::SetWindowIcon(Resources::GetResource(RES_SPRITE, "nugget.png").c_str());
    Application::RunApp(app);
    return 0;
}