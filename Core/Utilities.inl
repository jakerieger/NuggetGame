#pragma once

#include "Packer.h"
#include "Engine/GraphicsContext.h"
#include "Types.h"
#include "STL.h"

#include <optional>
#include <stb_image.h>
#include <GLFW/glfw3.h>

namespace Utilities {
    inline void HexToRGBA(const u32 hex, f32& r, f32& g, f32& b, f32& a) {
        const unsigned char alphaByte = (hex >> 24) & 0xFF;
        const unsigned char redByte   = (hex >> 16) & 0xFF;
        const unsigned char greenByte = (hex >> 8) & 0xFF;
        const unsigned char blueByte  = hex & 0xFF;

        a = static_cast<float>(static_cast<u32>(alphaByte) / 255.0);
        r = static_cast<float>(static_cast<u32>(redByte) / 255.0);
        g = static_cast<float>(static_cast<u32>(greenByte) / 255.0);
        b = static_cast<float>(static_cast<u32>(blueByte) / 255.0);
    }

    inline void HexToRGBA(const u32 hex, u32& r, u32& g, u32& b, u32& a) {
        const unsigned char alphaByte = (hex >> 24) & 0xFF;
        const unsigned char redByte   = (hex >> 16) & 0xFF;
        const unsigned char greenByte = (hex >> 8) & 0xFF;
        const unsigned char blueByte  = hex & 0xFF;

        a = static_cast<u32>(alphaByte);
        r = static_cast<u32>(redByte);
        g = static_cast<u32>(greenByte);
        b = static_cast<u32>(blueByte);
    }

    inline u32 RGBAToHex(const f32 r, const f32 g, const f32 b, const f32 a) {
        const auto redByte   = static_cast<unsigned char>(r * 255.0f);
        const auto greenByte = static_cast<unsigned char>(g * 255.0f);
        const auto blueByte  = static_cast<unsigned char>(b * 255.0f);
        const auto alphaByte = static_cast<unsigned char>(a * 255.0f);

        return (alphaByte << 24) | (redByte << 16) | (greenByte << 8) | blueByte;
    }

    template<typename T>
    T Lerp(T a, T b, double t) {
        if (a == b) {
            return a;
        }

        return a * (1.0 - t) + b * t;
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

    inline void SetWindowIcon(const Packer::Schemas::Sprite& iconSprite) {
        GLFWimage appIcon;
        appIcon.pixels = iconSprite.data;
        appIcon.width  = iconSprite.width;
        appIcon.height = iconSprite.height;
        glfwSetWindowIcon(Graphics::GetWindow(), 1, &appIcon);
        stbi_image_free(appIcon.pixels);
    }

    inline u32 LoadTextureFromFile(const char* path) {
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
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            stbi_image_free(data);
        } else {
            stbi_image_free(data);
        }

        return id;
    }

    inline u32 LoadTextureFromData(unsigned char* data, int width, int height, int channels) {
        u32 id;
        glGenTextures(1, &id);

        if (data) {
            GLenum format = GL_RGB;
            if (channels == 1) {
                format = GL_RED;
            } else if (channels == 3) {
                format = GL_RGB;
            } else if (channels == 4) {
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
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }

        return id;
    }
}  // namespace Utilities