#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class Loader;
class IndexBuffer;
class CommandBuffer;
class CommandPool;
class Buffer;
class PhysicalDevice;
class Device;


class VertexBuffer
{

public:

	// Vertex buffer
	VkBuffer m_Buffer;
	VkDeviceMemory m_Memory;
	VkDevice m_Device;

	VertexBuffer(PhysicalDevice* physicalDevice, Device* device, Loader* loader, IndexBuffer* indexBuffer, CommandPool* commandPool);

	~VertexBuffer();

};
