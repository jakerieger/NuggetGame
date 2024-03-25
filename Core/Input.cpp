//
// Created by jr on 2/12/24.
//

#include "Input.h"
#include "Engine/GameApp.h"
#include "Interfaces/InputListener.h"

#define GLFW_KEY_NONE 0

namespace Input {
    std::vector<IInputListener*> g_Listeners;

    void KeyCallback(
      GLFWwindow*, const int key, int, const int action, const int mods) {
        const u32 _key  = static_cast<u32>(key);
        const u32 _mods = static_cast<u32>(mods);
        FKeyEvent event {_key, _mods};

        for (const auto listener : g_Listeners) {
            if (action == GLFW_PRESS && listener) {
                listener->OnKeyDown(event);
            } else if (action == GLFW_RELEASE && listener) {
                listener->OnKeyUp(event);
            } else if (action == GLFW_REPEAT && listener) {
                listener->OnKey(event);
            }
        }
    }

    void MouseButtonCallback(GLFWwindow*,
                             const int button,
                             const int action,
                             const int) {
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

    void
    MouseMovementCallback(GLFWwindow*, const double xPos, const double yPos) {
        FMouseMoveEvent event {xPos, yPos};
        for (const auto listener : g_Listeners) {
            if (listener)
                listener->OnMouseMove(event);
        }
    }

    void
    ScrollCallback(GLFWwindow*, const double xOffset, const double yOffset) {
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
    }

    void RegisterListener(IInputListener* listener) {
        g_Listeners.push_back(listener);
    }

    void UnregisterSceneListeners(IInputListener* appListener) {
        // Hack for .clear() not clearing pointers for some reason (probably
        // just dumb). It will leave the pointers in the array and only clear
        // the memory associated with those pointers leading to 'Invalid
        // address' errors
        for (u32 i = 0; i < g_Listeners.size(); i++) {
            g_Listeners[i] = nullptr;
        }
        g_Listeners.clear();
        RegisterListener(appListener);
    }

}  // namespace Input