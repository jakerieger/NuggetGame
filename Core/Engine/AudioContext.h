// Author: Jake Rieger
// Created: 4/2/2024.
//

#pragma once

#include "STL.h"

namespace Audio {
    enum class EAudioTag : u8 {
        UI,
        FX,
        Music,
    };

    void Initialize();
    void PlayOneShot(const std::string& filename,
                     const std::string& channelName,
                     EAudioTag tag,
                     f32 gain = 1.f);
    void PlayLoop(const std::string& filename,
                  const std::string& channelName,
                  EAudioTag tag,
                  f32 gain = 1.f);
    void StopLoop(const std::string& channelName);
    void Shutdown();
}  // namespace Audio
