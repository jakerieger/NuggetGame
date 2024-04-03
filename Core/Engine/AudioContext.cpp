// Author: Jake Rieger
// Created: 4/2/2024.
//

#include "AudioContext.h"

#include "Logger.h"

#include <AL/al.h>
#include <AL/alext.h>
#include <AudioFile.h>

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

    void PlayOneShot(const std::string& filename) {
        AudioFile<f32> oneShot;
        if (!oneShot.load(filename)) {
            Logger::LogError(Logger::Subsystems::AUDIO,
                             "Failed to load audio file: '%s'",
                             filename.c_str());
        }

        const auto samples = oneShot.samples[0];
        unsigned int alSource;
        unsigned int alSampleSet;

        alGenSources(1, &alSource);
        alGenBuffers(1, &alSampleSet);
        alBufferData(alSampleSet,
                     AL_FORMAT_MONO_FLOAT32,
                     samples.data(),
                     static_cast<ALsizei>(samples.size()),
                     static_cast<ALsizei>(oneShot.getSampleRate()));
        alSourcei(alSource, AL_BUFFER, static_cast<ALint>(alSampleSet));

        oneShot.samples.clear();

        alSourcePlay(alSource);

        // Wait until sound has finished playing to delete the buffers
        // TODO: Figure out a way to do this asynchronously or with threads

        // alDeleteSources(1, &alSource);
        // alDeleteBuffers(1, &alSampleSet);
    }

    void Shutdown() {
        g_Context = alcGetCurrentContext();
        g_Device  = alcGetContextsDevice(g_Context);
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(g_Context);
        alcCloseDevice(g_Device);
    }
}  // namespace Audio