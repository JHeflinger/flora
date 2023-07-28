#include "flpch.h"
#include "Flora/Project/Project.h"
#include "AudioCommand.h"

namespace Flora {
	static ALCcontext* s_Context = nullptr;
	static ALCdevice* s_Device = nullptr;
    static std::unordered_map<ALuint, ALuint> s_BuffersToSources;

	namespace AudioUtils {

		static bool IsBigEndian() {
			int a = 1;
			return !((char*)&a)[0];
		}

		static int ConvertToInt(char* buffer, int len) {
			std::int32_t a = 0;
			if (!IsBigEndian())
				std::memcpy(&a, buffer, len);
			else
				for (std::size_t i = 0; i < len; ++i)
					reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
			return a;
		}
	}

    Audio AudioCommand::LoadWAV(std::string filepath) {
        Audio audio;
        audio.Filepath = filepath;
        std::ifstream file(filepath.c_str(), std::ios::binary);
        char buffer[4];
        if (!file.is_open()) {
            FL_CORE_ERROR("ERROR: Could not open audio file {}", filepath);
            return audio;
        }

        // the RIFF
        if (!file.read(buffer, 4)) {
            FL_CORE_ERROR("ERROR: could not read RIFF for audio file {}", filepath);
            return audio;
        }
        if (std::strncmp(buffer, "RIFF", 4) != 0) {
            FL_CORE_ERROR("ERROR: file is not a valid WAVE file (header doesn't begin with RIFF) for audio file {}", filepath);
            return audio;
        }

        // the size of the file
        if (!file.read(buffer, 4)) {
            FL_CORE_ERROR("ERROR: could not read size of file for audio file {}", filepath);
            return audio;
        }

        // the WAVE
        if (!file.read(buffer, 4)) {
            FL_CORE_ERROR("ERROR: could not read WAVE for audio file {}", filepath);
            return audio;
        }
        if (std::strncmp(buffer, "WAVE", 4) != 0) {
            FL_CORE_ERROR("ERROR: file is not a valid WAVE file (header doesn't contain WAVE) for audio file {}", filepath);
            return audio;
        }

        // "fmt/0"
        if (!file.read(buffer, 4)) {
            FL_CORE_ERROR("ERROR: could not read fmt/0 for audio file {}", filepath);
            return audio;
        }

        // this is always 16, the size of the fmt data chunk
        if (!file.read(buffer, 4)) {
            FL_CORE_ERROR("ERROR: could not read the 16 for audio file {}", filepath);
            return audio;
        }

        // PCM should be 1?
        if (!file.read(buffer, 2)) {
            FL_CORE_ERROR("ERROR: could not read PCM for audio file {}", filepath);
            return audio;
        }

        // the number of channels
        if (!file.read(buffer, 2)) {
            FL_CORE_ERROR("ERROR: could not read number of channels for audio file {}", filepath);
            return audio;
        }
        audio.Channels = (uint8_t)AudioUtils::ConvertToInt(buffer, 2);

        // sample rate
        if (!file.read(buffer, 4)) {
            FL_CORE_ERROR("ERROR: could not read sample rate for audio file {}", filepath);
            return audio;
        }
        audio.SampleRate = (uint32_t)AudioUtils::ConvertToInt(buffer, 4);

        // (sampleRate * bitsPerSample * channels) / 8
        if (!file.read(buffer, 4)) {
            FL_CORE_ERROR("ERROR: could not read (sampleRate * bitsPerSample * channels) / 8 for audio file {}", filepath);
            return audio;
        }

        // ?? dafaq
        if (!file.read(buffer, 2)) {
            FL_CORE_ERROR("ERROR: could not read dafaq for audio file {}", filepath);
            return audio;
        }

        // bitsPerSample
        if (!file.read(buffer, 2)) {
            FL_CORE_ERROR("ERROR: could not read bits per sample for audio file {}", filepath);
            return audio;
        }
        audio.BPS = (uint8_t)AudioUtils::ConvertToInt(buffer, 2);

        // data chunk header "data"
        if (!file.read(buffer, 4)) {
            FL_CORE_ERROR("ERROR: could not read data chunk header for audio file {}", filepath);
            return audio;
        }
        if (std::strncmp(buffer, "data", 4) != 0) {
            FL_CORE_ERROR("ERROR: file is not a valid WAVE file (doesn't have 'data' tag) for audio file {}", filepath);
            return audio;
        }

        // size of data
        if (!file.read(buffer, 4)) {
            FL_CORE_ERROR("ERROR: could not read data size for audio file {}", filepath);
            return audio;
        }
        audio.Size = (ALsizei)AudioUtils::ConvertToInt(buffer, 4);

        /* cannot be at the end of file */
        if (file.eof()) {
            FL_CORE_ERROR("ERROR: reached EOF on the file for audio file {}", filepath);
            return audio;
        }
        if (file.fail()) {
            FL_CORE_ERROR("ERROR: fail state set on the file for audio file {}", filepath);
            return audio;
        }

        audio.Data = new char[audio.Size];
        file.read(audio.Data, audio.Size);
        audio.Initialized = true;

        alGenBuffers(1, &audio.BufferID);
        if (audio.Channels == 1)
            if (audio.BPS == 8)
                audio.Format = AL_FORMAT_MONO8;
            else
                audio.Format = AL_FORMAT_MONO16;
        else
            if (audio.BPS == 8)
                audio.Format = AL_FORMAT_STEREO8;
            else
                audio.Format = AL_FORMAT_STEREO16;
        alBufferData(audio.BufferID, audio.Format, audio.Data, audio.Size, audio.SampleRate);
        alGenSources(1, &audio.SourceID);

        return audio;
    }

	void AudioCommand::Init() {
        ALCdevice* s_Device = alcOpenDevice(NULL);
        if (s_Device == NULL) {
            FL_CORE_ERROR("cannot open sound card");
            return;
        }
        ALCcontext* s_Context = alcCreateContext(s_Device, NULL);
        if (s_Context == NULL) {
            FL_CORE_ERROR("cannot open audio context");
            return;
        }
        alcMakeContextCurrent(s_Context);
	}

	void AudioCommand::Shutdown() {
		alcDestroyContext(s_Context);
		alcCloseDevice(s_Device);
	}

    void AudioCommand::Play(Audio& audio, float scale, float pitch, float gain, glm::vec3 velocity, bool loop, glm::vec3 location) {
        Update(audio, scale, pitch, gain, velocity, loop, location);
        alSourcePlay(audio.SourceID);
    }

    void AudioCommand::Update(Audio& audio, float scale, float pitch, float gain, glm::vec3 velocity, bool loop, glm::vec3 location) {
        alSourcef(audio.SourceID, AL_PITCH, pitch);
        alSourcef(audio.SourceID, AL_GAIN, gain);
        alSource3f(audio.SourceID, AL_POSITION, location.x * scale, location.y * scale, location.z * scale);
        alSource3f(audio.SourceID, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
        alSourcei(audio.SourceID, AL_LOOPING, loop);
        alSourcei(audio.SourceID, AL_BUFFER, audio.BufferID);
    }

    void AudioCommand::Stop(Audio& audio) {
        alSourceStop(audio.SourceID);
    }

    void AudioCommand::Delete(Audio& audio) {
        alDeleteSources(1, &audio.SourceID);
        alDeleteBuffers(1, &audio.BufferID);
    }

    void AudioCommand::Test() {
        Audio audio = LoadWAV(Project::GetAssetDirectory().string() + "/Audio/cringe.wav");
        Play(audio);
    }
}