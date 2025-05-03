#define VMA_IMPLEMENTATION
#include <external/VulkanMemoryAllocator/vk_mem_alloc.h>
#include "vkAllocator.h"
#undef VMA_IMPLEMENTATION

Allocator::Allocator(VkInstance* instance, VkPhysicalDevice* physDevice, VkDevice* device)
{
	VmaAllocatorCreateInfo allocInfo = {};
	allocInfo.instance = *instance;
	allocInfo.physicalDevice = *physDevice;
	allocInfo.device = *device;
	allocInfo.vulkanApiVersion = VK_API_VERSION_1_3;
	allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
	vmaCreateAllocator(&allocInfo, &_allocator);
}

BufferAlloc Allocator::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VmaAllocationCreateFlags memFlags, VmaMemoryUsage memUsage) const
{
	BufferAlloc newBuffer{};
	VkBuffer newB;
	VmaAllocation newA;

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;

	VmaAllocationCreateInfo memoryInfo{};
	memoryInfo.flags = memFlags;
	memoryInfo.usage = memUsage;

	vmaCreateBuffer(_allocator, &bufferInfo, &memoryInfo, &newB, &newA, nullptr);
	newBuffer.buffer = &newB;
	newBuffer.alloc = &newA;
	return newBuffer;
}


void Allocator::copyIntoAllocation(VmaAllocation allocation, void* data, VkDeviceSize offset, VkDeviceSize size) const
{
	vmaCopyMemoryToAllocation(_allocator, data, allocation, offset, size);
}

void Allocator::copyFromAllocation(VmaAllocation allocation, void** data, VkDeviceSize offset, VkDeviceSize size) const
{
	vmaCopyAllocationToMemory(_allocator, allocation, offset, data, size);
}

void Allocator::mapMemory(VmaAllocation allocation, void** data) const
{
	vmaMapMemory(_allocator, allocation, data);
}

void Allocator::unmapMemory(VmaAllocation allocation) const
{
	vmaUnmapMemory(_allocator, allocation);
}

void Allocator::destroy(BufferAlloc* buffer) const
{
	vmaDestroyBuffer(_allocator, *buffer->buffer, *buffer->alloc);
}

void Allocator::destroy(ImageAlloc* image) const
{
	vmaDestroyImage(_allocator, *image->image, *image->alloc);
}


Allocator::~Allocator()
{
	vmaDestroyAllocator(_allocator);
}
