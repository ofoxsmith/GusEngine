#pragma once

#include <external/VulkanMemoryAllocator/vk_mem_alloc.h>
#include <vulkan/vulkan.h>
namespace vkAllocator {
	struct Alloc {
		bool inUse = false;
		bool mapped = false;
		VmaAllocation alloc;
		VmaAllocationInfo info;
	};

	struct BufferAlloc : public Alloc {
		VkBuffer buffer;
	};

	struct ImageAlloc : public Alloc {
		VkImage image;
	};

	class Allocator {
		VmaAllocator _allocator;

		public:
		Allocator(VkInstance* instance, VkPhysicalDevice* physDevice, VkDevice* device);
		void createBuffer(BufferAlloc* alloc, VkDeviceSize size, VkBufferUsageFlags usage, VmaAllocationCreateFlags memFlags = 0, VmaMemoryUsage memUsage = VMA_MEMORY_USAGE_AUTO) const;
		void createImage(ImageAlloc* alloc);


		void copyIntoAllocation(Alloc* allocation, void* data, VkDeviceSize offset, VkDeviceSize size) const;
		void copyFromAllocation(Alloc* allocation, void** data, VkDeviceSize offset, VkDeviceSize size) const;
		void mapMemory(Alloc* allocation, void** data) const;
		void unmapMemory(Alloc* allocation) const;

		void destroy(BufferAlloc* buffer) const;
		void destroy(ImageAlloc* image) const;
		~Allocator();
	};
}