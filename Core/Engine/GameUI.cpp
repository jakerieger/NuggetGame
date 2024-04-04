// Author: Jake Rieger
// Created: 3/28/2024.
//

#include "GameUI.h"

#include "GraphicsContext.h"
#include "Logger.h"
#include "Resources.h"
#include "Utilities.inl"

#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include <RmlUi/RmlUi_Platform_GLFW.h>
#include <RmlUi/RmlUi_Renderer_GL3.h>

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
              Utilities::JoinPath(Resources::GetRoot(), "Assets", "fonts", "Square.ttf").string(),
              false)) {
            throw std::runtime_error("Failed to load font");
        }

        Logger::LogInfo(Logger::Subsystems::UI, "UI subsystem initialized.");
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

    Rml::ElementDocument* CreateDocument(const std::string& source) {
        Rml::ElementDocument* document = g_Context->LoadDocumentFromMemory(source, "");
        return document;
    }

    void CloseDocument(Rml::ElementDocument* document) { document->Close(); }
}  // namespace UI