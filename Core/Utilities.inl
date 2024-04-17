#pragma once

#include "Packer.h"
#include "Engine/GraphicsContext.h"
#include "Types.h"
#include "STL.h"

#include <optional>
#include <stb_image.h>
#include <GLFW/glfw3.h>

namespace Utilities {
    namespace fs = std::filesystem;

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
        appIcon.width  = static_cast<int>(iconSprite.width);
        appIcon.height = static_cast<int>(iconSprite.height);
        glfwSetWindowIcon(Graphics::GetWindow(), 1, &appIcon);
        stbi_image_free(appIcon.pixels);
    }

    inline u32 LoadTextureFromFile(const char* path) {
        u32 id;
        glGenTextures(1, &id);

        i32 width, height, nrComponents;
        stbi_set_flip_vertically_on_load(true);
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
                         static_cast<int>(format),
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

    inline u32 LoadTextureFromData(const unsigned char* data,
                                   const unsigned int width,
                                   const unsigned int height,
                                   const unsigned int channels) {
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
                         static_cast<int>(format),
                         static_cast<int>(width),
                         static_cast<int>(height),
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

    template<typename T1, typename T2>
    bool BitCompare(T1 a, T2 b) {
        const auto A = *(char*)&a;
        const auto B = *(char*)&b;
        return (A ^ B) == 0;
    }

    inline bool CheckTolerance(const f32 v1, const f32 v2, const f32 t) {
        const auto upperBound = v2 + t;
        const auto lowerBound = v2 - t;
        if (v1 >= lowerBound && upperBound >= v1) {
            return true;
        }
        return false;
    }

    inline fs::path JoinPath(const fs::path& root, const std::string& first) {
        return root / first;
    }

    template<typename... Args>
    fs::path JoinPath(const fs::path& root, const std::string& first, const Args&... rest) {
        return JoinPath(root / first, rest...);
    }

    template<typename T>
    std::vector<T> InterleaveVectors(const std::vector<T>& vec1, const std::vector<T>& vec2) {
        std::vector<T> result;
        const size_t size = std::min(vec1.size(), vec2.size());

        for (size_t i = 0; i < size; ++i) {
            result.push_back(vec1[i]);
            result.push_back(vec2[i]);
        }

        // Add any remaining elements from the longer vector
        for (size_t i = size; i < vec1.size(); ++i) {
            result.push_back(vec1[i]);
        }
        for (size_t i = size; i < vec2.size(); ++i) {
            result.push_back(vec2[i]);
        }

        return result;
    }

    inline i32 MakeMultiple(const i32 number, const i32 multiple) {
        const i32 remainder  = number % multiple;
        const i32 difference = multiple - remainder;
        return number + difference;
    }

    template<typename T>
    void RemoveAt(std::vector<T>& vec, i32 idx) {
        if (idx >= 0 && idx < vec.size()) {
            vec.erase(vec.begin() + idx);
        }
    }
}  // namespace Utilities