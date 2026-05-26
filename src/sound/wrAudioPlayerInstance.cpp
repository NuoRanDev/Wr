#include "wrAudioPlayerInstance.hpp"

#include "log/wrLogOutput.hpp"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace wr
{
	bool AudioPlayerInstance::init() noexcept
	{
		// device  : ALCdevice*
		// context : ALCcontext*
		device = alcOpenDevice(nullptr);
		if (!device) 
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrSound : OpenAL", "Fail to open device");
			return false;
		}
		context = alcCreateContext(reinterpret_cast<ALCdevice*>(device), nullptr);
		if (!context) 
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrSound : OpenAL", "Fail to create context");
			return false;
		}
		alcMakeContextCurrent(reinterpret_cast<ALCcontext*>(context));
		if (alGetError() != AL_NO_ERROR) 
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrSound : OpenAL", "Fail to make current context");
			return false;
		}
		return true;
	}

	AudioPlayerInstance::~AudioPlayerInstance()
	{
		alcDestroyContext(reinterpret_cast<ALCcontext*>(context));
		alcCloseDevice(reinterpret_cast<ALCdevice*>(device));
	}
} // namespace wr is end