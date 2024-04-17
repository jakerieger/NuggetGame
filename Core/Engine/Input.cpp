//
// Created by jr on 2/12/24.
//

#include "Input.h"
#include "GameApp.h"
#include "GameUI.h"
#include "Logger.h"
#include "../Interfaces/InputListener.h"

#include <Utilities.inl>
#include <thread>
#include <RmlUi/Core/Context.h>

namespace Input {
    static std::vector<IInputListener*> g_Listeners;
    static IInputListener* g_AppListener = nullptr;
    int g_ConnectedGamepad;

    struct FKeyState {
        bool Pressed  = false;
        bool Released = false;
    };

    std::unordered_map<u32, FKeyState> g_KeyStates;
    std::thread g_DispatcherThread;

    /// Required to maintain OnKey events once a new key has been pressed.
    /// GLFW can only register a single key per frame.
    void Dispatcher() {
        while (Application::IsRunning()) {
            if (!Application::IsLoading()) {
                for (const auto& [key, state] : g_KeyStates) {
                    // Check if the key is pressed
                    if (state.Pressed) {
                        // Dispatch OnKey event to listeners
                        FKeyEvent event {key, 0};
                        for (const auto listener : g_Listeners) {
                            listener->OnKey(event);
                        }
                    }
                }
                // Sleep for a short duration (120 FPS) to avoid busy waiting
                std::this_thread::sleep_for(std::chrono::milliseconds(8));
            }
        }
    }

    /// https://discourse.glfw.org/t/key-callback-not-registering-every-key-press/1438/4
    void KeyCallback(GLFWwindow*, const int key, int, const int action, const int mods) {
        const u32 _key  = static_cast<u32>(key);
        const u32 _mods = static_cast<u32>(mods);
        FKeyEvent event {_key, _mods};

        if (action == GLFW_PRESS) {
            g_KeyStates[key].Pressed  = true;
            g_KeyStates[key].Released = false;
        } else if (action == GLFW_RELEASE) {
            g_KeyStates[key].Pressed  = false;
            g_KeyStates[key].Released = true;
        }

        if (g_KeyStates[key].Pressed) {
            for (const auto listener : g_Listeners) {
                listener->OnKeyDown(event);
            }
            g_AppListener->OnKeyDown(event);
        } else if (g_KeyStates[key].Released) {
            for (const auto listener : g_Listeners) {
                listener->OnKeyUp(event);
            }
            g_AppListener->OnKeyUp(event);
        }
    }

    void GamepadCallback(int joystick, int event) {
        if (event == GLFW_CONNECTED) {
            g_ConnectedGamepad = joystick;
        } else {
            g_ConnectedGamepad = -1;
        }
    }

    void MouseButtonCallback(GLFWwindow*, const int button, const int action, const int) {
        const u32 _button = static_cast<u32>(button);
        FMouseEvent event {_button};

        if (action == GLFW_PRESS) {
            for (const auto listener : g_Listeners) {
                listener->OnMouseDown(event);
            }

            UI::GetContext()->ProcessMouseButtonDown(button, 0);
            g_AppListener->OnMouseDown(event);
        } else if (action == GLFW_RELEASE) {
            for (const auto listener : g_Listeners) {
                listener->OnMouseUp(event);
            }

            UI::GetContext()->ProcessMouseButtonUp(button, 0);
            g_AppListener->OnMouseUp(event);
        }
    }

    void MouseMovementCallback(GLFWwindow*, const double xPos, const double yPos) {
        FMouseMoveEvent event {xPos, yPos};
        for (const auto listener : g_Listeners) {
            if (listener)
                listener->OnMouseMove(event);
        }
        UI::GetContext()->ProcessMouseMove(static_cast<i32>(xPos), static_cast<i32>(yPos), 0);
        g_AppListener->OnMouseMove(event);
    }

    void ScrollCallback(GLFWwindow*, const double xOffset, const double yOffset) {
        FScrollEvent event {xOffset, yOffset};

        for (const auto listener : g_Listeners) {
            if (listener)
                listener->OnScroll(event);
        }
        UI::GetContext()->ProcessMouseWheel(static_cast<f32>(yOffset), 0);
        g_AppListener->OnScroll(event);
    }

    void Initialize(GLFWwindow* window, IInputListener* appListener) {
        g_AppListener = appListener;

        glfwSetKeyCallback(window, KeyCallback);
        glfwSetMouseButtonCallback(window, MouseButtonCallback);
        glfwSetCursorPosCallback(window, MouseMovementCallback);
        glfwSetScrollCallback(window, ScrollCallback);
        glfwSetJoystickCallback(GamepadCallback);

        g_DispatcherThread = std::thread(Dispatcher);

        Logger::LogInfo(Logger::Subsystems::INPUT, "Input subsystem initialized.");
    }

    void RegisterListener(IInputListener* listener) {
        if (listener)
            g_Listeners.push_back(listener);
    }

    void UnregisterListener(const IInputListener* listener) {
        if (listener) {
            if (const auto it = std::ranges::find_if(
                  g_Listeners,
                  [&](const IInputListener* elem) { return elem == listener; });
                it != g_Listeners.end()) {
                const auto idx      = std::distance(g_Listeners.begin(), it);
                g_Listeners.at(idx) = nullptr;
                Utilities::RemoveAt(g_Listeners, static_cast<i32>(idx));
            }
        }
    }

    void Shutdown() {
        g_DispatcherThread.join();
    }

}  // namespace Input