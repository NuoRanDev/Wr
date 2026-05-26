#ifndef _INC_WR_SOUND_SOURCE_HPP_
#define _INC_WR_SOUND_SOURCE_HPP_

#include "audio/wrAudio.hpp"
#include "math/wrMathVector.hpp"

namespace wr
{
	class SoundSource
	{
	public:
		SoundSource() noexcept = default;

		bool lood_audio(PcmBlock pcm) noexcept;

		void set_is_loop(bool is_loop) noexcept;

		void set_position(const vec3f position) noexcept;

		void set_velocity(const vec3f velocity) noexcept;

		void set_pitch(float pitch) noexcept;

		void set_gain(float gain) noexcept;

		int32_t play();

		void release() noexcept;

	private:
		uint32_t source, buffer;
	};
} // namespace wr is end

#endif // _INC_WR_SOUND_SOURCE_HPP_ IS EOF