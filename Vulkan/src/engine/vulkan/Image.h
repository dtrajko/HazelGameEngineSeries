#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "PhysicalDevice.h"

#include <vector>


class PhysicalDevice;
class Buffer;
class CommandBuffer;
class CommandPool;
class SwapChain;
class ImageView;
class Format;


class Image
{
public:

	VkDevice m_Device;

	VkImage m_Image;
	VkDeviceMemory m_ImageMemory;
	ImageView* m_ImageView;


public:

	Image(VkDevice device, PhysicalDevice* physicalDevice,
		uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties);

	ImageView* createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

	~Image();
};
