// Author: Jake Rieger
// Created: 4/17/2024.
//

#include "Cursor.h"

#include <Engine/GraphicsContext.h>
#include <GLFW/glfw3.h>

namespace Cursor {
    bool g_Visible = true;

    void Hide() {
        glfwSetInputMode(Graphics::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        g_Visible = false;
    }

    void Show() {
        glfwSetInputMode(Graphics::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        g_Visible = true;
    }
}  // namespace Cursor