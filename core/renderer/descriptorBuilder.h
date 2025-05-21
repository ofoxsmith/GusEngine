#pragma once
#include <vulkan/vulkan.h>
#include <external/vkBootstrap/VkBootstrap.h>
#include "core/globals.h"
#include <vector>

// Descriptor sets and pools will all be unqiue to a single layout
class DescriptorAllocator {
	private:
	std::vector<VkDescriptorSetLayoutBinding> _bindings;
	
	VkDescriptorSetLayout _layout;
	VkDescriptorPool _pool = nullptr;

	public:
	DescriptorAllocator(VkDevice _device, std::vector<VkDescriptorSetLayoutBinding> layoutBindings, VkDescriptorSetLayoutCreateFlags layoutCreateFlags = 0) {
		
		_bindings = std::vector<VkDescriptorSetLayoutBinding>(layoutBindings);
		
		for (size_t i = 0; i < _bindings.size(); i++)
		{
			_bindings[i].binding = i;
		}
		
		VkDescriptorSetLayoutCreateInfo info = { .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
		info.flags = layoutCreateFlags;
		info.pBindings = _bindings.data();
		info.bindingCount = _bindings.size();

		if (vkCreateDescriptorSetLayout(_device, &info, nullptr, &_layout) != VK_SUCCESS) {
			Log.FatalError("Vulkan", "Failed to build descriptor set layout");
		}

	}

	void CreatePool(VkDevice device, uint32_t totalLayouts=1) {
		VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = static_cast<unsigned int>(MAX_FRAMES_IN_FLIGHT);
		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;
		poolInfo.maxSets = static_cast<unsigned int>(MAX_FRAMES_IN_FLIGHT);
		if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
			Log.FatalError("Vulkan", "failed to create descriptor pool!");
		}

	}


	VkDescriptorSetLayout GetLayoutObj() const {
		return _layout;
	}
};