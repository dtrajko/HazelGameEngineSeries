#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class Loader;
class Buffer;
class CommandBuffer;
class CommandPool;
class PhysicalDevice;


class IndexBuffer
{

public:

	VkBuffer m_Buffer;
	VkDeviceMemory m_Memory;
	VkDevice m_Device;

	IndexBuffer(PhysicalDevice* physicalDevice, VkDevice device, Loader* loader, Buffer* buffer,
		VkQueue graphicsQueue, CommandPool* commandPool);

	~IndexBuffer();

	void copyBuffer(VkDevice device, VkQueue graphicsQueue, CommandPool* commandPool,
		VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

};
