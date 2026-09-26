#ifndef _INC_WR_GRAPHICS_INSTANCE_HPP_
#define _INC_WR_GRAPHICS_INSTANCE_HPP_

// core
#include <type/wrResult.hpp>
#include <string/wrString.hpp>
#include <math/wrMathVector.hpp>
// std
#include <functional>

namespace wr
{

	enum class ChooseGPUSolution
	{
		NAME = 0,
	};

	ResultInfo init_wr_graphics_instance(String app_name, ChooseGPUSolution solution) noexcept;

	any_type_ptr_t get_vk_inst() noexcept;

	void set_direct_display_mode() noexcept;

	ResultInfo create_render_surface(any_type_ptr_t bitmap, vec2u bit_map_size) noexcept;
} // namespace wr is end

#endif // _INC_WR_GRAPHICS_INSTANCE_HPP_ IS EOF