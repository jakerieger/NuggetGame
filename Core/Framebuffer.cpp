//
// Created by conta on 2/21/2024.
//

#include "Framebuffer.h"

#include "Engine/GraphicsContext.h"

#include <iostream>
#include <glad/glad.h>

void AFramebuffer::Initialize() {
    glGenFramebuffers(1, &m_FrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

    glGenTextures(1, &m_TexColorBuffer);
    glBindTexture(GL_TEXTURE_2D, m_TexColorBuffer);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 Graphics::GetWindowSize().at(0),
                 Graphics::GetWindowSize().at(1),
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D,
                           m_TexColorBuffer,
                           0);

    glGenRenderbuffers(1, &m_RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER,
                          GL_DEPTH24_STENCIL8,
                          Graphics::GetWindowSize().at(0),
                          Graphics::GetWindowSize().at(1));  // use a single renderbuffer object for
                                                             // both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER,
                              m_RBO);  // now actually attach it
    // now that we actually created the framebuffer and added all attachments we
    // want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AFramebuffer::Bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    glEnable(GL_DEPTH_TEST);
}

void AFramebuffer::Shutdown() const {
    glDeleteRenderbuffers(1, &m_RBO);
    glDeleteFramebuffers(1, &m_FrameBuffer);
}