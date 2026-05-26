#ifndef _INC_FRAGMENTSHADER_HPP_
#define _INC_FRAGMENTSHADER_HPP_

#include "type/wrOrdinals.hpp"
#include "string/wrString.hpp"

#include "vulkan/vulkan.h"

#include "log/wrLogOutput.hpp"

namespace wr
{
	/*
		namespace vulkan
	{
		// NOTE: THE SHADER COMMAND IS TEMP DATA IN MMAP FROM FILE
		// THIS CLASS NOT DELETE SHARDE DATA
		class ShaderGroup final
		{
		public:
			ShaderGroup(const byte_t* vert_data, size_t vert_data_size, const byte_t* frag_data, size_t frag_data_size) noexcept
			{
				vk_vert_mod = nullptr;
				vk_frag_mod = nullptr;

				vk_vert_mod_info = {};
				vk_vert_mod_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				vk_vert_mod_info.pCode = reinterpret_cast<const uint32_t*>(vert_data);
				vk_vert_mod_info.codeSize = vert_data_size;

				vk_frag_mod_info = {};
				vk_frag_mod_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				vk_frag_mod_info.pCode = reinterpret_cast<const uint32_t*>(vert_data);
				vk_frag_mod_info.codeSize = vert_data_size;
			}

			// default function is main
			void create_shader_process_info(VkDevice vk_device, const char* frag_enter = "main", const char* vert_enter = "main")
			{
				if (vk_frag_mod_info.pCode != nullptr)
				{
					if (vkCreateShaderModule(vk_device, &(vk_frag_mod_info), nullptr, &vk_frag_mod) != VK_SUCCESS)
					{
						XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "wrGraphicsAPI", "failed to load frag spir-v file!")
					}
				}
				if (vk_vert_mod_info.pCode != nullptr)
				{
					if (vkCreateShaderModule(vk_device, &(vk_vert_mod_info), nullptr, &vk_vert_mod) != VK_SUCCESS)
					{
						XE_FATAL_OUTPUT(XE_TYPE_NAME_OUTPUT::APP, "wrGraphicsAPI", "failed to load vert spir-v file!")
					}
				}
				shader_stage_info[0] =
				{
					.stage = VK_SHADER_STAGE_VERTEX_BIT,
					.pName = vert_enter,
					.module = vk_vert_mod
				};
				shader_stage_info[1] =
				{
					.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
					.pName = frag_enter,
					.module = vk_frag_mod
				};
			}

			void release(VkDevice vk_device)
			{
				vkDestroyShaderModule(vk_device, vk_frag_mod, nullptr);
				vkDestroyShaderModule(vk_device, vk_vert_mod, nullptr);
			}

			VkPipelineShaderStageCreateInfo shader_stage_info[2];
			
			VkShaderModuleCreateInfo vk_vert_mod_info;
			VkShaderModuleCreateInfo vk_frag_mod_info;

			VkShaderModule vk_vert_mod;
			VkShaderModule vk_frag_mod;
		};
	}// namespace vulkan is end
	*/

} // namespace wr is end


#endif // !_INC_FRAGMENTSHADER_HPP_ IS EOF