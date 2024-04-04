// Author: Jake Rieger
// Created: 4/2/2024.
//

#include "AudioContext.h"

#include "Logger.h"
#include "Utilities.inl"

#include <AL/al.h>
#include <AL/alext.h>
#include <AudioFile.h>
#include <future>

namespace Audio {
    ALCdevice* g_Device;
    ALCcontext* g_Context;
    ALCboolean g_ContextCurrent = false;

    static bool CheckALErrors() {
        if (const ALenum error = alGetError(); error != AL_NO_ERROR) {
            switch (error) {
                case AL_INVALID_NAME:
                    Logger::LogError(
                      Logger::Subsystems::AUDIO,
                      "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function");
                    break;
                case AL_INVALID_ENUM:
                    Logger::LogError(
                      Logger::Subsystems::AUDIO,
                      "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function");
                    break;
                case AL_INVALID_VALUE:
                    Logger::LogError(
                      Logger::Subsystems::AUDIO,
                      "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function");
                    break;
                case AL_INVALID_OPERATION:
                    Logger::LogError(Logger::Subsystems::AUDIO,
                                     "AL_INVALID_OPERATION: the requested operation is not valid");
                    break;
                case AL_OUT_OF_MEMORY:
                    Logger::LogError(Logger::Subsystems::AUDIO,
                                     "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL "
                                     "running out of memory");
                    break;
                default:
                    Logger::LogError(Logger::Subsystems::AUDIO, "UNKNOWN AL ERROR: %s", error);
            }
            return false;
        }
        return true;
    }

    static bool CheckALCErrors(ALCdevice* device) {
        if (const ALCenum error = alcGetError(device); error != ALC_NO_ERROR) {
            switch (error) {
                case ALC_INVALID_VALUE:
                    Logger::LogError(
                      Logger::Subsystems::AUDIO,
                      "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function");
                    break;
                case ALC_INVALID_DEVICE:
                    Logger::LogError(
                      Logger::Subsystems::AUDIO,
                      "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function");
                    break;
                case ALC_INVALID_CONTEXT:
                    Logger::LogError(
                      Logger::Subsystems::AUDIO,
                      "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function");
                    break;
                case ALC_INVALID_ENUM:
                    Logger::LogError(
                      Logger::Subsystems::AUDIO,
                      "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function");
                    break;
                case ALC_OUT_OF_MEMORY:
                    Logger::LogError(
                      Logger::Subsystems::AUDIO,
                      "ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL "
                      "function");
                    break;
                default:
                    Logger::LogError(Logger::Subsystems::AUDIO, "UNKNOWN ALC ERROR: %s", error);
            }
            return false;
        }
        return true;
    }

    void Initialize() {
        const auto deviceName = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);

        g_Device = alcOpenDevice(deviceName);
        if (!g_Device) {
            Logger::LogError(Logger::Subsystems::AUDIO, "Could not open AL device.");
        }

        g_Context        = alcCreateContext(g_Device, nullptr);
        g_ContextCurrent = alcMakeContextCurrent(g_Context);
        if (!g_ContextCurrent) {
            Logger::LogError(Logger::Subsystems::AUDIO, "Failed to make AL context current.");
        }

        CheckALCErrors(g_Device);
        CheckALErrors();

        Logger::LogInfo(Logger::Subsystems::AUDIO, "Audio subsystem initialized.");
    }

    static std::tuple<u32, u32>
    PlaySoundFile(const std::string& filename, const bool loop = false, const f32 gain = 1.f) {
        AudioFile<f32> oneShot;
        oneShot.shouldLogErrorsToConsole(false);
        if (!oneShot.load(filename)) {
            Logger::LogError(Logger::Subsystems::AUDIO,
                             "Failed to load audio file: '%s'",
                             filename.c_str());
        }

        ALenum format;
        if (oneShot.isMono()) {
            format = AL_FORMAT_MONO_FLOAT32;
        } else {
            format = AL_FORMAT_STEREO_FLOAT32;
        }

        u32 alSource;
        u32 alSampleSet;
        std::vector<f32> samples;
        samples.reserve(oneShot.getNumSamplesPerChannel() * oneShot.getNumChannels());

        if (oneShot.isMono()) {
            samples = oneShot.samples.at(0);
        } else {
            samples = Utilities::InterleaveVectors(oneShot.samples.at(0), oneShot.samples.at(1));
        }

        samples.resize(
          Utilities::MakeMultiple(static_cast<i32>(samples.size()),
                                  static_cast<i32>(sizeof(f32)) * oneShot.getNumChannels()),
          0.f);

        alGenSources(1, &alSource);
        alGenBuffers(1, &alSampleSet);
        alBufferData(alSampleSet,
                     format,
                     samples.data(),
                     static_cast<ALsizei>(samples.size()),
                     static_cast<ALsizei>(oneShot.getSampleRate()));
        alSourcei(alSource, AL_BUFFER, static_cast<ALint>(alSampleSet));
        alSourcei(alSource, AL_LOOPING, loop);
        alSourcef(alSource, AL_GAIN, gain);

        oneShot.samples.clear();

        alSourcePlay(alSource);

        return std::make_tuple(alSource, alSampleSet);
    }

    static void Cleanup(const u32 source, const u32 buffer) {
        ALint sourceState;
        do {
            alGetSourcei(source, AL_SOURCE_STATE, &sourceState);
        } while (sourceState == AL_PLAYING);
        alDeleteSources(1, &source);
        alDeleteBuffers(1, &buffer);
    }

    void PlayOneShot(const std::string& filename, const f32 gain) {
        const std::tuple<u32, u32> result = PlaySoundFile(filename, false, gain);
        const auto source                 = std::get<0>(result);
        const auto buffer                 = std::get<1>(result);

        auto cleanupThread = std::thread(Cleanup, source, buffer);
        cleanupThread.detach();
    }

    void PlayLoop(const std::string& filename, const f32 gain) {
        const std::tuple<u32, u32> result = PlaySoundFile(filename, true, gain);
        const auto source                 = std::get<0>(result);
        const auto buffer                 = std::get<1>(result);
    }

    void Shutdown() {
        g_Context = alcGetCurrentContext();
        g_Device  = alcGetContextsDevice(g_Context);
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(g_Context);
        alcCloseDevice(g_Device);
    }
}  // namespace Audio