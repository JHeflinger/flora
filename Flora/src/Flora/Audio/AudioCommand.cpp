#include "flpch.h"
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
			int a = 0;
			if (!IsBigEndian())
				for (int i = 0; i < len; i++)
					((char*)&a)[i] = buffer[i];
			else
				for (int i = 0; i < len; i++)
					((char*)&a)[3 - i] = buffer[i];
			return a;
		}

		static char* LoadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size) {
			char buffer[4];
			std::ifstream in(fn, std::ios::binary);
			in.read(buffer, 4);
			if (strncmp(buffer, "RIFF", 4) != 0)
			{
				std::cout << "this is not a valid WAVE file" << std::endl;
				return NULL;
			}
			in.read(buffer, 4);
			in.read(buffer, 4);      //WAVE
			in.read(buffer, 4);      //fmt
			in.read(buffer, 4);      //16
			in.read(buffer, 2);      //1
			in.read(buffer, 2);
			chan = ConvertToInt(buffer, 2);
			in.read(buffer, 4);
			samplerate = ConvertToInt(buffer, 4);
			in.read(buffer, 4);
			in.read(buffer, 2);
			in.read(buffer, 2);
			bps = ConvertToInt(buffer, 2);
			in.read(buffer, 4);      //data
			in.read(buffer, 4);
			size = ConvertToInt(buffer, 4);
			char* data = new char[size];
			in.read(data, size);
			return data;
		}
	}

	void AudioCommand::Init() {
        int channel, sampleRate, bps, size;
        char* data = AudioUtils::LoadWAV("assets/sounds/ExampleWAV.wav", channel, sampleRate, bps, size);
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