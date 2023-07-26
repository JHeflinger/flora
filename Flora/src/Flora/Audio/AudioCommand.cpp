#include "flpch.h"
#include "Flora/Project/Project.h"
#include "AudioCommand.h"
#include "alc.h"
#include "al.h"

namespace Flora {

	static ALCcontext* s_Context = nullptr;
	static ALCdevice* s_Device = nullptr;

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

		static char* LoadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size) {
			
            std::ifstream file(fn, std::ios::binary);
			char buffer[4];
            if (!file.is_open()) {
                FL_CORE_ERROR("ERROR: Could not open audio file {}", fn);
                return '\0';
            }

            // the RIFF
            if(!file.read(buffer, 4)) {
                FL_CORE_ERROR("ERROR: could not read RIFF for audio file {}", fn);
                return '\0';
            }
            if(std::strncmp(buffer, "RIFF", 4) != 0) {
                FL_CORE_ERROR("ERROR: file is not a valid WAVE file (header doesn't begin with RIFF) for audio file {}", fn);
                return '\0';
            }

            // the size of the file
            if(!file.read(buffer, 4)) {
                FL_CORE_ERROR("ERROR: could not read size of file for audio file {}", fn);
                return '\0';
            }

            // the WAVE
            if(!file.read(buffer, 4)) {
                FL_CORE_ERROR("ERROR: could not read WAVE for audio file {}", fn);
                return '\0';;
            }
            if(std::strncmp(buffer, "WAVE", 4) != 0) {
                FL_CORE_ERROR("ERROR: file is not a valid WAVE file (header doesn't contain WAVE) for audio file {}", fn);
                return '\0';
            }

            // "fmt/0"
            if(!file.read(buffer, 4)) {
                FL_CORE_ERROR("ERROR: could not read fmt/0 for audio file {}", fn);
                return '\0';
            }

            // this is always 16, the size of the fmt data chunk
            if(!file.read(buffer, 4)) {
                FL_CORE_ERROR("ERROR: could not read the 16 for audio file {}", fn);
                return '\0';
            }

            // PCM should be 1?
            if(!file.read(buffer, 2)) {
                FL_CORE_ERROR("ERROR: could not read PCM for audio file {}", fn);
                return '\0';
            }

            // the number of channels
            if(!file.read(buffer, 2)) {
                FL_CORE_ERROR("ERROR: could not read number of channels for audio file {}", fn);
                return '\0';
            }
            chan = ConvertToInt(buffer, 2);

            // sample rate
            if(!file.read(buffer, 4)) {
                FL_CORE_ERROR("ERROR: could not read sample rate for audio file {}", fn);
                return '\0';
            }
            samplerate = ConvertToInt(buffer, 4);

            // (sampleRate * bitsPerSample * channels) / 8
            if(!file.read(buffer, 4)) {
                FL_CORE_ERROR("ERROR: could not read (sampleRate * bitsPerSample * channels) / 8 for audio file {}", fn);
                return '\0';
            }

            // ?? dafaq
            if(!file.read(buffer, 2)) {
                FL_CORE_ERROR("ERROR: could not read dafaq for audio file {}", fn);
                return '\0';
            }

            // bitsPerSample
            if(!file.read(buffer, 2)) {
                FL_CORE_ERROR("ERROR: could not read bits per sample for audio file {}", fn);
                return '\0';
            }
            bps = ConvertToInt(buffer, 2);

            // data chunk header "data"
            if(!file.read(buffer, 4)) {
                FL_CORE_ERROR("ERROR: could not read data chunk header for audio file {}", fn);
                return '\0';
            }
            if(std::strncmp(buffer, "data", 4) != 0) {
                FL_CORE_ERROR("ERROR: file is not a valid WAVE file (doesn't have 'data' tag) for audio file {}", fn);
                return '\0';
            }

            // size of data
            if(!file.read(buffer, 4)) {
                FL_CORE_ERROR("ERROR: could not read data size for audio file {}", fn);
                return '\0';
            }
            size = ConvertToInt(buffer, 4);

            /* cannot be at the end of file */
            if(file.eof()) {
                FL_CORE_ERROR("ERROR: reached EOF on the file for audio file {}", fn);
                return '\0';
            }
            if(file.fail()) {
                FL_CORE_ERROR("ERROR: fail state set on the file for audio file {}", fn);
                return '\0';
            }

            char* data = new char[size];
            file.read(data, size);
            return data;
		}
	}

	void AudioCommand::Init() {
        int channel, sampleRate, bps, size;
        char* data = AudioUtils::LoadWAV((Project::GetAssetDirectory().string() + "/Audio/phone_wav.wav").c_str(), channel, sampleRate, bps, size);
		FL_CORE_INFO("data: {}, sampleRate: {}, bps: {}, size: {}", channel, sampleRate, bps, size);
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

        unsigned int bufferid, format;
        alGenBuffers(1, &bufferid);
        if (channel == 1) {
            if (bps == 8) {
                format = AL_FORMAT_MONO8;
            } else {
                format = AL_FORMAT_MONO16;
            }
        } else {
			if (bps == 8){
                format = AL_FORMAT_STEREO8;
            } else {
                format = AL_FORMAT_STEREO16;
            }
        }
        alBufferData(bufferid, format, data, size, sampleRate);
        unsigned int sourceid;
        alGenSources(1, &sourceid);
        alSourcei(sourceid, AL_BUFFER, bufferid);
        alSourcePlay(sourceid);
	}

	void AudioCommand::Shutdown() {
		//alDeleteSources(1, &sourceid);
		//alDeleteBuffers(1, &bufferid);

		//alcDestroyContext(context);
		//alcCloseDevice(device);
		//delete[] data;
	}
}