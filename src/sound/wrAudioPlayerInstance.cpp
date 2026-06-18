// statement
#include <wrAudioPlayerInstance.hpp>
// core
#include <log/wrLogOutput.hpp>
// openal
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace wr
{
	ResultInfo AudioPlayerInstance::init() noexcept
	{
		// device  : ALCdevice*
		// context : ALCcontext*
		device = alcOpenDevice(nullptr);
		if (!device) 
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrSound : OpenAL", "Fail to open device");
			return ResultInfo::WR_ERROR;
		}
		context = alcCreateContext(reinterpret_cast<ALCdevice*>(device), nullptr);
		if (!context) 
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrSound : OpenAL", "Fail to create context");
			return ResultInfo::WR_ERROR;
		}
		alcMakeContextCurrent(reinterpret_cast<ALCcontext*>(context));
		if (alGetError() != AL_NO_ERROR) 
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrSound : OpenAL", "Fail to make current context");
			return ResultInfo::WR_ERROR;
		}
		return ResultInfo::WR_OK;
	}

	AudioPlayerInstance::~AudioPlayerInstance()
	{
		alcDestroyContext(reinterpret_cast<ALCcontext*>(context));
		alcCloseDevice(reinterpret_cast<ALCdevice*>(device));
	}
} // namespace wr is end