#ifndef _INC_WR_AUDIO_PLAYER_INSTANCE_HPP_
#define _INC_WR_AUDIO_PLAYER_INSTANCE_HPP_

#include "type/wrDataStruction.hpp"

namespace wr 
{
	class AudioPlayerInstance
	{
	public:
		AudioPlayerInstance()
		{
			device = nullptr;
			context = nullptr;
		}

		bool init() noexcept;

		~AudioPlayerInstance();

	private:

		dynamic_array<std::pair<uint32_t, uint32_t>> src_buf;

		void* device;
		void* context;
	};

} // namespace wr is end

#endif // _INC_WR_AUDIO_PLAYER_INSTANCE_HPP_ IS EOF