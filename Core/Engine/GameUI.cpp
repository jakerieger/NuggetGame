// Author: Jake Rieger
// Created: 3/28/2024.
//

#include "GameUI.h"

#include "GraphicsContext.h"
#include "Resources.h"

#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include <RmlUi/RmlUi_Platform_GLFW.h>
#include <RmlUi/RmlUi_Renderer_GL3.h>

static const std::string TestDocument = R""(<rml>
<head>
	<title>Window</title>
	<style>
		body {
			width: 100vw;
			height: 100vh;
			margin: auto;
            font-family: Share Tech Mono;
            text-align: right;
            font-size: 20dp;
		}
	</style>
</head>
<body class="window">
    <h1>Pause Menu</h1>
</body>
</rml>)"";

namespace UI {
    SystemInterface_GLFW* g_SysInterface   = nullptr;
    RenderInterface_GL3* g_RenderInterface = nullptr;
    Rml::Context* g_Context                = nullptr;

    void Initialize() {
        Rml::String renderMsg;
        if (!RmlGL3::Initialize(&renderMsg)) {
            throw std::runtime_error(renderMsg);
        }

        g_SysInterface    = new SystemInterface_GLFW();
        g_RenderInterface = new RenderInterface_GL3();

        g_SysInterface->SetWindow(Graphics::GetWindow());
        g_SysInterface->LogMessage(Rml::Log::LT_INFO, renderMsg);

        const auto windowSize = Graphics::GetWindowSize();
        g_RenderInterface->SetViewport(windowSize.at(0), windowSize.at(1));

        Rml::SetSystemInterface(g_SysInterface);
        Rml::SetRenderInterface(g_RenderInterface);
        if (!Rml::Initialise()) {
            throw std::runtime_error("Failed to initialize RmlUi");
        }

        g_Context = Rml::CreateContext("main", Rml::Vector2i(windowSize.at(0), windowSize.at(1)));
        if (!g_Context) {
            Shutdown();
        }

        if (!Rml::Debugger::Initialise(g_Context)) {
            throw std::runtime_error("Failed to initialize RmlUi Debugger");
        }

        // Load font
        if (!Rml::LoadFontFace(
              (Resources::GetRoot() / "Assets/fonts/ShareTechMono-Regular.ttf").string(),
              false)) {
            throw std::runtime_error("Failed to load font");
        }

        if (Rml::ElementDocument* document = g_Context->LoadDocumentFromMemory(TestDocument, "")) {
            document->Show();
        }
    }

    void Begin() {
        const auto windowSize = Graphics::GetWindowSize();
        g_Context->SetDimensions(Rml::Vector2i(windowSize.at(0), windowSize.at(1)));
        g_RenderInterface->SetViewport(windowSize.at(0), windowSize.at(1));
        g_Context->Update();
        g_RenderInterface->BeginFrame();
    }

    void Draw() { g_Context->Render(); }

    void End() { g_RenderInterface->EndFrame(); }

    void Shutdown() {
        Rml::Shutdown();
        RmlGL3::Shutdown();

        delete g_SysInterface;
        delete g_RenderInterface;
    }

    void LoadDocument(const std::string& source) {}
}  // namespace UI