// Author: Jake Rieger
// Created: 4/2/2024.
//

#include "AudioContext.h"

#include "EngineSettings.h"
#include "Logger.h"
#include "Utilities.inl"

#include <AL/al.h>
#include <AL/alext.h>
#include <AudioFile.h>
#include <future>

namespace Audio {
    struct FAudioChannel {
        u32 Source;
        u32 Buffer;
        std::string Name;
    };

    ALCdevice* g_Device;
    ALCcontext* g_Context;
    ALCboolean g_ContextCurrent = false;
    static std::vector<FAudioChannel> g_Mixer;
    // Amplitude target for normalization
    static constexpr f32 TARGET_PEAK = 0.99f;

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

    static void Normalize(std::vector<f32>& samples) {
        if (samples.empty())
            return;

        f32 maxAbsValue = 0.f;
        for (const f32 sample : samples) {
            maxAbsValue = std::max(maxAbsValue, std::abs(sample));
        }
        const f32 scale = TARGET_PEAK / maxAbsValue;
        for (f32& sample : samples) {
            sample *= scale;
        }
    }

    static std::tuple<u32, u32> PlaySoundFile(const std::string& filename,
                                              EAudioTag tag,
                                              const bool loop = false,
                                              const f32 gain  = 1.f) {
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

        // OpenAL requires that the number of samples be a multiple of the data type size in bytes
        // (float in this case) * the number of channels. If it's not, this line adds additional
        // silence at the end of the buffer to make it a multiple.
        samples.resize(
          Utilities::MakeMultiple(static_cast<i32>(samples.size()),
                                  static_cast<i32>(sizeof(f32)) * oneShot.getNumChannels()),
          0.f);

        Normalize(samples);

        auto finalVolume = gain * Settings::GetSettings().Audio.VolumeMaster;
        switch (tag) {
            case EAudioTag::Music: {
                finalVolume *= Settings::GetSettings().Audio.VolumeMusic;
                break;
            }
            case EAudioTag::UI: {
                finalVolume *= Settings::GetSettings().Audio.VolumeUI;
                break;
            }
            case EAudioTag::FX: {
                finalVolume *= Settings::GetSettings().Audio.VolumeFX;
                break;
            }
        }

        alGenSources(1, &alSource);
        alGenBuffers(1, &alSampleSet);
        alBufferData(alSampleSet,
                     format,
                     samples.data(),
                     static_cast<ALsizei>(samples.size()),
                     static_cast<ALsizei>(oneShot.getSampleRate()));
        alSourcei(alSource, AL_BUFFER, static_cast<ALint>(alSampleSet));
        alSourcei(alSource, AL_LOOPING, loop);
        alSourcef(alSource, AL_GAIN, finalVolume);

        oneShot.samples.clear();

        alSourcePlay(alSource);

        return std::make_tuple(alSource, alSampleSet);
    }

    void PlayOneShot(const std::string& filename,
                     const std::string& channelName,
                     const EAudioTag tag,
                     const f32 gain) {
        const std::tuple<u32, u32> result = PlaySoundFile(filename, tag, false, gain);
        const auto source                 = std::get<0>(result);
        const auto buffer                 = std::get<1>(result);
        g_Mixer.emplace_back(source, buffer, channelName);
    }

    void PlayLoop(const std::string& filename,
                  const std::string& channelName,
                  const EAudioTag tag,
                  const f32 gain) {
        const std::tuple<u32, u32> result = PlaySoundFile(filename, tag, true, gain);
        const auto source                 = std::get<0>(result);
        const auto buffer                 = std::get<1>(result);
        g_Mixer.emplace_back(source, buffer, channelName);
    }

    void StopLoop(const std::string& channelName) {
        const auto it = std::ranges::find_if(g_Mixer, [&](const FAudioChannel& channel) {
            return channel.Name == channelName;
        });

        if (it == g_Mixer.end()) {
            return;
        }

        auto [source, buffer, _] = *it;
        alDeleteSources(1, &source);
        alDeleteBuffers(1, &buffer);
        g_Mixer.erase(it);
    }

    void Shutdown() {
        for (auto& [source, buffer, _] : g_Mixer) {
            alDeleteSources(1, &source);
            alDeleteBuffers(1, &buffer);
        }
        g_Mixer.clear();

        g_Context = alcGetCurrentContext();
        g_Device  = alcGetContextsDevice(g_Context);
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(g_Context);
        alcCloseDevice(g_Device);
    }
}  // namespace Audio