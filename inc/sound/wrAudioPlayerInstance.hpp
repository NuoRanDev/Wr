#ifndef _INC_WR_AUDIO_PLAYER_INSTANCE_HPP_
#define _INC_WR_AUDIO_PLAYER_INSTANCE_HPP_
// core
#include <type/wrDataStruction.hpp>
#include <type/wrResult.hpp>

namespace wr
{

	ResultInfo init_audio_instance() noexcept;

	void free_audio_instance() noexcept;


} // namespace wr is end

#endif // _INC_WR_AUDIO_PLAYER_INSTANCE_HPP_ IS EOF