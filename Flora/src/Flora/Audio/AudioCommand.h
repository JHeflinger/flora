#pragma once
#include "alc.h"
#include "al.h"
#include "glm/glm.hpp"

namespace Flora {
    struct Audio {
        uint8_t Channels = 0;
        uint32_t SampleRate = 0;
        uint8_t BPS = 0;
        ALsizei Size = 0;
        char* Data = nullptr;
        bool Initialized = false;
        std::string Filepath = "";
        ALuint BufferID = 0;
        ALuint Format = 0;
        ALuint SourceID = 0;
    };

	class AudioCommand {
	public:
		static void Init();
		static void Shutdown();
        static void Play(Audio& audio, float scale = 1.0f, float pitch = 1.0f, float gain = 1.0f, glm::vec3 velocity = { 0.0f, 0.0f, 0.0f }, bool loop = false, glm::vec3 location = { 0.0f, 0.0f, 0.0f });
        static void Update(Audio& audio, float scale = 1.0f, float pitch = 1.0f, float gain = 1.0f, glm::vec3 velocity = { 0.0f, 0.0f, 0.0f }, bool loop = false, glm::vec3 location = { 0.0f, 0.0f, 0.0f });
        static void Stop(Audio& audio);
        static void Delete(Audio& audio);
        static Audio LoadWAV(std::string filepath);
        static void Test();
	};
}

/*
ALuint source;
alCall(alGenSources, 1, &source);
alCall(alSourcef, source, AL_PITCH, 1);
alCall(alSourcef, source, AL_GAIN, 1.0f);
alCall(alSource3f, source, AL_POSITION, 0, 0, 0);
alCall(alSource3f, source, AL_VELOCITY, 0, 0, 0);
alCall(alSourcei, source, AL_LOOPING, AL_FALSE);
alCall(alSourcei, source, AL_BUFFER, buffer);
*/