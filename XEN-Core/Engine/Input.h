//
// Created by jr on 2/12/24.
//

#pragma once
#include "../Interfaces/InputListener.h"
#include "../STL.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Input {
    void Initialize(GLFWwindow* window, IInputListener* appListener);
    void RegisterListener(IInputListener* listener);
    void UnregisterListener(const IInputListener* listener);
    void Shutdown();
}  // namespace Input