#pragma once

#include "Engine/GraphicsContext.h"
#include "Types.h"
#include "STL.h"

#include <optional>
#include <stb_image.h>
#include <GLFW/glfw3.h>

namespace Utilities {
    inline void HexToRGBAf(const u32 hex, float& r, float& g, float& b, float& a) {
        const unsigned char alphaByte = (hex >> 24) & 0xFF;
        const unsigned char redByte   = (hex >> 16) & 0xFF;
        const unsigned char greenByte = (hex >> 8) & 0xFF;
        const unsigned char blueByte  = hex & 0xFF;

        a = static_cast<float>(static_cast<u32>(alphaByte) / 255.0);
        r = static_cast<float>(static_cast<u32>(redByte) / 255.0);
        g = static_cast<float>(static_cast<u32>(greenByte) / 255.0);
        b = static_cast<float>(static_cast<u32>(blueByte) / 255.0);
    }

    inline std::optional<std::string> ReadFileToString(const char* path) noexcept {
        if (FILE* fp = fopen(path, "rb")) {
            std::string contents;
            fseek(fp, 0, SEEK_END);
            contents.resize(ftell(fp));
            rewind(fp);
            fread(&contents[0], 1, contents.size(), fp);
            fclose(fp);
            return contents;
        }
        return {};
    }

    inline void SetWindowIcon(const char* file) {
        GLFWimage appIcon;
        appIcon.pixels = stbi_load(file, &appIcon.width, &appIcon.height, nullptr, 4);
        glfwSetWindowIcon(Graphics::GetWindow(), 1, &appIcon);
        stbi_image_free(appIcon.pixels);
    }

    inline u32 LoadTexture(const char* path) {
        u32 id;
        glGenTextures(1, &id);

        i32 width, height, nrComponents;
        u8* data = stbi_load(path, &width, &height, &nrComponents, 0);
        if (data) {
            GLenum format = GL_RGB;
            if (nrComponents == 1) {
                format = GL_RED;
            } else if (nrComponents == 3) {
                format = GL_RGB;
            } else if (nrComponents == 4) {
                format = GL_RGBA;
            }

            glBindTexture(GL_TEXTURE_2D, id);
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         format,
                         width,
                         height,
                         0,
                         format,
                         GL_UNSIGNED_BYTE,
                         data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D,
                            GL_TEXTURE_WRAP_S,
                            format == GL_RGBA
                              ? GL_CLAMP_TO_EDGE
                              : GL_REPEAT);  // for this tutorial: use GL_CLAMP_TO_EDGE to
                                             // prevent semi-transparent borders. Due to
                                             // interpolation it takes texels from next repeat
            glTexParameteri(GL_TEXTURE_2D,
                            GL_TEXTURE_WRAP_T,
                            format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        } else {
            stbi_image_free(data);
        }

        return id;
    }
}  // namespace Utilities