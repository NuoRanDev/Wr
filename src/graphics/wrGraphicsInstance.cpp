// statement
#include <wrGraphicsInstance.hpp>
// graphics
#include <vulkan/wrVulkanConfig.hpp>
#include <vulkan/wrVulkan.hpp>
// core
#include <memory/wrAlloc.hpp>
#include <log/wrLogOutput.hpp>

namespace wr
{
	static VulkanContext* vk_ctx;

	ResultInfo init_wr_graphics_instance(String app_name, ChooseGPUSolution solution) noexcept
	{
		uint32_t score = -1;
		// malloc vk_ctx
		vk_ctx = wr_malloc<VulkanContext>(1);
		// set null in ptr*
		// set 0 in count
		init_vk_ctx(vk_ctx);
		// init vulkan instance
		// not show window
		// if failed , will exit this program
		init_vulkan_instance(vk_ctx, app_name.data());
		find_gpu(vk_ctx);
		// select gpu
		// if not , will exit this program
		find_gpu(vk_ctx);
		switch(solution)
		{
		case ChooseGPUSolution::NAME:
			goto NAME_SOLUTION;
		default:
			goto NO_SOLUTION;
		}
		NAME_SOLUTION:
		// usually the nvidia's GPU is better than other
		// what is more, we select AMD GPU and Intel GPU
		// in the end NO:0 GPU will be selected
		for (uint32_t i = 0; i < vk_ctx->gpu_cout; i++)
		{
			if (strstr(vk_ctx->vk_gpu_properties[i].deviceName, "NVIDIA"))
			{
				vk_ctx->cur_used_gpu_index = i;
				break;
			}
			if (strstr(vk_ctx->vk_gpu_properties[i].deviceName, "AMD"))
			{
				vk_ctx->cur_used_gpu_index = i;
				score = 2;
			}
			if ((strstr(vk_ctx->vk_gpu_properties[i].deviceName, "INTEL")) && (score > 1))
			{
				vk_ctx->cur_used_gpu_index = i;
				score = 1;
			}
		}
		if (vk_ctx->cur_used_gpu_index != -1)
			goto END;
		else { goto NO_SOLUTION; }

		NO_SOLUTION:
			vk_ctx->cur_used_gpu_index = 0;
		vk_ctx->cur_used_gpu = vk_ctx->gpu_list[vk_ctx->cur_used_gpu_index];
		return ResultInfo::WR_WARNING;

		END:
		WR_CLR_WRITE_LINE(std::format("Use the {0}", vk_ctx->vk_gpu_properties[vk_ctx->cur_used_gpu_index].deviceName).c_str());
		return ResultInfo::WR_OK;
	}

	any_type_ptr_t get_vk_inst() noexcept
	{
		return reinterpret_cast<any_type_ptr_t>(vk_ctx->vk_main_instance);
	}

	void set_direct_display_mode() noexcept
	{
		vk_ctx->is_direct_display = true;
	}

	ResultInfo create_render_surface(any_type_ptr_t bitmap, vec2u bit_map_size) noexcept
	{
		vk_ctx->bitmap_surface = reinterpret_cast<VkSurfaceKHR>(bitmap);
		if (create_logic_device(vk_ctx, 0, false))
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrGraphics", "Create vulkan logic device failed!");
			return ResultInfo::WR_ERROR;
		}
		if (create_swapchain(vk_ctx, bit_map_size, 3, true, true))
		{
			WR_ERROR_OUTPUT(WR_TYPE_NAME_OUTPUT::APP, "wrGraphics", "Create vulkan logic device failed!");
			return ResultInfo::WR_ERROR;
		}
		return ResultInfo::WR_OK;
	}

	ResultInfo render_surface_resize()
	{
		return ResultInfo::WR_OK;
	}
} // namespace wr is end