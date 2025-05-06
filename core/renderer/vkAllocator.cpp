#define VMA_IMPLEMENTATION
#include <external/VulkanMemoryAllocator/vk_mem_alloc.h>
#include "vkAllocator.h"
#include "core/globals.h"
#undef VMA_IMPLEMENTATION
using namespace vkAllocator;
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

void Allocator::createBuffer(BufferAlloc* alloc, VkDeviceSize size, VkBufferUsageFlags usage, VmaAllocationCreateFlags memFlags, VmaMemoryUsage memUsage) const
{
	if (alloc->inUse) {
		Log.Error("VMA", "Attempted to create buffer using a BufferAlloc object that is already in use.");
		return;
	}
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;

	VmaAllocationCreateInfo memoryInfo{};
	memoryInfo.flags = memFlags;
	memoryInfo.usage = memUsage;

	alloc->inUse = true;
	VkResult r = vmaCreateBuffer(_allocator, &bufferInfo, &memoryInfo, &alloc->buffer, &alloc->alloc, &alloc->info);
}

void Allocator::createImage(ImageAlloc* alloc, ImageParams params, VkExtent3D extent, uint32_t flags, VmaAllocationCreateFlags memFlags, VmaMemoryUsage memUsage) const
{
	if (alloc->inUse) {
		Log.Error("VMA", "Attempted to create image using an ImageAlloc object that is already in use.");
		return;
	}
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	imageInfo.usage = params.usage;
	imageInfo.format = params.format;
	imageInfo.tiling = params.tiling;
	imageInfo.initialLayout = params.layout;
	imageInfo.sharingMode = params.sharingMode;
	imageInfo.samples = params.samples;
	imageInfo.extent = extent;
	imageInfo.flags = flags;
	VmaAllocationCreateInfo memoryInfo{};
	memoryInfo.flags = memFlags;
	memoryInfo.usage = memUsage;
	
	alloc->inUse = true;
	alloc->format = params.format;
	alloc->extent = extent;
	VkResult r = vmaCreateImage(_allocator, &imageInfo, &memoryInfo, &alloc->image, &alloc->alloc, &alloc->info);

}

void Allocator::copyIntoAllocation(Alloc* allocation, void* data, VkDeviceSize offset, VkDeviceSize size) const
{
	vmaCopyMemoryToAllocation(_allocator, data, allocation->alloc, offset, size);
}

void Allocator::copyFromAllocation(Alloc* allocation, void** data, VkDeviceSize offset, VkDeviceSize size) const
{
	vmaCopyAllocationToMemory(_allocator, allocation->alloc, offset, data, size);
}

void Allocator::mapMemory(Alloc* allocation, void** data) const
{
	allocation->mapped = true;
	vmaMapMemory(_allocator, allocation->alloc, data);
}

void Allocator::unmapMemory(Alloc* allocation) const
{
	vmaUnmapMemory(_allocator, allocation->alloc);
	allocation->mapped = false;
}

void Allocator::destroy(BufferAlloc* buffer) const
{
	if (buffer->mapped) unmapMemory(buffer);
	vmaDestroyBuffer(_allocator, buffer->buffer, buffer->alloc);
	buffer->inUse = false;
}

void Allocator::destroy(ImageAlloc* image) const
{
	if (image->mapped) unmapMemory(image);
	vmaDestroyImage(_allocator, image->image, image->alloc);
	image->inUse = false;
}


Allocator::~Allocator()
{
	vmaDestroyAllocator(_allocator);
}
