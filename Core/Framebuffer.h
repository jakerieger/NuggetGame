//
// Created by conta on 2/21/2024.
//

#pragma once

#include "STL.h"

class AFramebuffer {
public:
    AFramebuffer() : m_FrameBuffer(0), m_TexColorBuffer(0), m_RBO(0) {}

    void Initialize();
    void Bind() const;
    void Shutdown() const;

    u32 GetTextureBuffer() const {
        return m_TexColorBuffer;
    }

private:
    u32 m_FrameBuffer;
    u32 m_TexColorBuffer;
    u32 m_RBO;
};