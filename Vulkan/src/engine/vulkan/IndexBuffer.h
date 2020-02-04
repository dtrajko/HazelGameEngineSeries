#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class Loader;
class Buffer;
class CommandBuffer;
class CommandPool;
class PhysicalDevice;
class Device;


class IndexBuffer
{

public:

	VkBuffer m_Buffer;
	VkDeviceMemory m_Memory;
	VkDevice m_Device;

	IndexBuffer(PhysicalDevice* physicalDevice, Device* device, Loader* loader, Buffer* buffer, CommandPool* commandPool);

	~IndexBuffer();

	void copyBuffer(Device* device, CommandPool* commandPool,
		VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

};
