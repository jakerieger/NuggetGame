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
            font-size: 20dp;
		}

        .background {
            background-color: #000000;
            opacity: 0.5;
			width: 100vw;
			height: 100vh;
            text-align: center;
            position: fixed;
            top: 0;
            left: 0;
            z-index: -1;
        }

        .panel {
            width: 300dp;
            height: 200dp;
            text-align: center;
            padding: 20dp;
        }

        .container {
            display: flex;
            align-items: center;
            justify-content: center;
            height: 100%;
        }

        input[type="button"] {
            width: 300dp;
            min-height: 30dp;
            background-color: #202020;
            line-height: 20dp;
            text-align: center;
            padding: 16dp;
        }

        form {
            width: 300dp;
            background-color: #e6b038;
            display: flex;
            flex-direction: column;
        }

        h1 {
            font-size: 30dp;
            font-effect: glow(2dp #ed5);
        }
	</style>
</head>
<body class="window">
    <div class="background"/>
    <div class="container">
        <div>
            <div class="panel">
                <form>
                    <h1>PAUSE MENU</h1>
                    <input type="button">MAIN MENU</input>
                    <input type="button"button>QUIT</input>
                </form>
            </div>
        </div>
    </div>
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

        // if (Rml::ElementDocument* document = g_Context->LoadDocumentFromMemory(TestDocument, ""))
        // {
        //     document->Show();
        // }
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