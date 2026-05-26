#include "wrSoundSource.hpp"

#include <AL/al.h>
#include <AL/alext.h>

#include "log/wrLogOutput.hpp"
#include "memory/wrAlloc.hpp"

namespace wr
{

	bool SoundSource::lood_audio(PcmBlock pcm) noexcept 
	{
		alGenBuffers(1, &buffer);
		alGenSources(1, &source);
		alBufferData(buffer, AL_FORMAT_MONO_FLOAT32, pcm.pcm_data, pcm.data_size, pcm.header.sample_rate);
		alSourcei(source, AL_BUFFER, buffer);

		if (alGetError() != AL_NO_ERROR)
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::LIB, "wrSound : OpenAL", "Fail to laod pcm data");
			return false;
		}
		wr_free(pcm.pcm_data);
		return true;
	}

	void SoundSource::set_is_loop(bool is_loop) noexcept 
	{
		alSourcei(source, AL_LOOPING, static_cast<ALint>(is_loop));
	}

	void SoundSource::set_position(const vec3f position) noexcept 
	{
		alSourcefv(source, AL_POSITION, reinterpret_cast<const float*>(&position));
	}

	void SoundSource::set_velocity(const vec3f velocity) noexcept 
	{
		alSourcefv(source, AL_VELOCITY, reinterpret_cast<const float*>(&velocity));
	}

	void SoundSource::set_pitch(float pitch) noexcept
	{
		alSourcef(source, AL_PITCH, pitch);
	}

	void SoundSource::set_gain(float gain) noexcept
	{
		alSourcef(source, AL_GAIN, gain);
	}

	int32_t SoundSource::play()
	{
		ALint state;
		alGetSourcei(source, AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING)
		{
			return 0;
		}
		else
		{
			alSourcePlay(source);
			if (alGetError() != AL_NO_ERROR)
			{
				return -1;
			}
			return 0;
		}
	}

	void SoundSource::release() noexcept
	{
		alDeleteSources(1, &source);
		alDeleteBuffers(1, &buffer);
	}

} // namespace wr is end
