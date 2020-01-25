#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class CommandBuffer
{
public:

	VkCommandBuffer m_CommandBuffer;
	VkCommandPool m_CommandPool;
	VkDevice m_Device;
	// VkQueue m_Queue;

public:

	CommandBuffer(VkDevice device, VkCommandPool commandPool, uint32_t count);
	~CommandBuffer();
	void Record();
	void QueueSubmit(VkQueue queue, uint32_t count);

};
