//
// Created by jr on 2/12/24.
//

#include "Input.h"
#include "GameApp.h"
#include "Logger.h"
#include "../Interfaces/InputListener.h"

#include <thread>

namespace Input {
    std::vector<IInputListener*> g_Listeners;
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

        for (const auto listener : g_Listeners) {
            if (g_KeyStates[key].Pressed) {
                listener->OnKeyDown(event);
            } else if (g_KeyStates[key].Released) {
                listener->OnKeyUp(event);
            }
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

        for (const auto listener : g_Listeners) {
            if (action == GLFW_PRESS && listener) {
                listener->OnMouseDown(event);
            } else if (action == GLFW_RELEASE && listener) {
                listener->OnMouseUp(event);
            }
        }
    }

    void MouseMovementCallback(GLFWwindow*, const double xPos, const double yPos) {
        FMouseMoveEvent event {xPos, yPos};
        for (const auto listener : g_Listeners) {
            if (listener)
                listener->OnMouseMove(event);
        }
    }

    void ScrollCallback(GLFWwindow*, const double xOffset, const double yOffset) {
        FScrollEvent event {xOffset, yOffset};

        for (const auto listener : g_Listeners) {
            if (listener)
                listener->OnScroll(event);
        }
    }

    void Initialize(GLFWwindow* window) {
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetMouseButtonCallback(window, MouseButtonCallback);
        glfwSetCursorPosCallback(window, MouseMovementCallback);
        glfwSetScrollCallback(window, ScrollCallback);
        glfwSetJoystickCallback(GamepadCallback);

        g_DispatcherThread = std::thread(Dispatcher);

        Logger::LogInfo(Logger::Subsystems::INPUT, "Input subsystem initialized.");
    }

    void RegisterListener(IInputListener* listener) { g_Listeners.push_back(listener); }

    void UnregisterSceneListeners(IInputListener* appListener) {
        // Hack for .clear() not clearing pointers for some reason (I'm probably
        // just dumb). It will leave the pointers in the array and only clear
        // the memory associated with those pointers leading to 'Invalid
        // address' errors.
        for (auto& listener : g_Listeners) {
            listener = nullptr;
        }

        // Remove all the active input listeners
        g_Listeners.clear();
        // Re-register the main app as a listener
        RegisterListener(appListener);
    }

    void Shutdown() { g_DispatcherThread.join(); }

}  // namespace Input