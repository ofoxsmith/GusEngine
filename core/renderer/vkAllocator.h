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
		VkImageView imageView;
		VkFormat format;
		VkExtent3D extent;
		VkImageLayout layout;
	};

	// The allocator class provides a system for creating and manipulating buffers/images and their memory allocations.
	class Allocator {
		VmaAllocator _allocator;

		public:
		struct ImageParams {
			VkImageType type;
			VkImageUsageFlags usage;
			VkFormat format;
			VkImageTiling tiling;
			VkImageLayout layout;
			VkSharingMode sharingMode;
			VkSampleCountFlagBits samples;
		};

		VkDevice* _device;
		Allocator(VkInstance* instance, VkPhysicalDevice* physDevice, VkDevice* device);
		void createBuffer(BufferAlloc* alloc, VkDeviceSize size, VkBufferUsageFlags usage, VmaAllocationCreateFlags memFlags = 0, VmaMemoryUsage memUsage = VMA_MEMORY_USAGE_AUTO) const;
		void createImage(ImageAlloc* alloc, ImageParams params, VkExtent3D extent, uint32_t flags = 0, VmaAllocationCreateFlags memFlags = 0, VmaMemoryUsage memUsage = VMA_MEMORY_USAGE_AUTO) const;
		void copyIntoAllocation(Alloc* allocation, void* data, VkDeviceSize offset, VkDeviceSize size) const;
		void copyFromAllocation(Alloc* allocation, void** data, VkDeviceSize offset, VkDeviceSize size) const;

		void copyBufferToBufferCmd(VkCommandBuffer buffer, BufferAlloc* src, BufferAlloc* dst, VkDeviceSize size, bool freeOldBuffer = false) const;
		void copyBufferToImageCmd(VkCommandBuffer buffer, BufferAlloc* src, ImageAlloc* dst, VkExtent3D extent, bool freeOldBuffer = false);
		void transitionImageLayoutCmd(VkCommandBuffer cmdBuffer, ImageAlloc* image, VkImageLayout oldLayout, VkImageLayout newLayout);
		void mapMemory(Alloc* allocation, void** data) const;
		void unmapMemory(Alloc* allocation) const;

		void destroy(BufferAlloc* buffer) const;
		void destroy(ImageAlloc* image) const;
		~Allocator();
	};
}