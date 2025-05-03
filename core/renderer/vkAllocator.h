#pragma once

#include <external/VulkanMemoryAllocator/vk_mem_alloc.h>
#include <vulkan/vulkan.h>

struct BufferAlloc {
	VkBuffer* buffer;
	VmaAllocation* alloc;
};

struct ImageAlloc {
	VkImage* image;
	VmaAllocation* alloc;

};

class Allocator {
	VmaAllocator _allocator;

	public:
	Allocator(VkInstance* instance, VkPhysicalDevice* physDevice, VkDevice* device);
	BufferAlloc createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VmaAllocationCreateFlags memFlags = 0, VmaMemoryUsage memUsage = VMA_MEMORY_USAGE_AUTO) const;
	ImageAlloc createImage();
	

	void copyIntoAllocation(VmaAllocation allocation, void* data, VkDeviceSize offset, VkDeviceSize size) const;
	void copyFromAllocation(VmaAllocation allocation, void** data, VkDeviceSize offset, VkDeviceSize size) const;
	void mapMemory(VmaAllocation allocation, void** data) const;
	void unmapMemory(VmaAllocation allocation) const;

	void destroy(BufferAlloc* buffer) const;
	void destroy(ImageAlloc* image) const;
	~Allocator();
};