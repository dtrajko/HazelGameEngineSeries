#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class PhysicalDevice;


class Buffer
{

public:

	VkBuffer m_Buffer;
	VkDeviceMemory m_Memory;
	VkDevice m_Device;

public:

	Buffer(PhysicalDevice* physicalDevice, VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	~Buffer();

};
