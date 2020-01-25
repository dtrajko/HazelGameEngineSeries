#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class ImageView
{

public:

	VkImageView m_ImageView;
	VkDevice m_Device;

public:

	ImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	~ImageView();

};
