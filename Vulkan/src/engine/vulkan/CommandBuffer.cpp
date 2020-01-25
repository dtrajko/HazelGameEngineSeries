#include "CommandBuffer.h"

#include <stdexcept>


CommandBuffer::CommandBuffer(VkDevice device, VkCommandPool commandPool, uint32_t count)
	: m_Device(device), m_CommandPool(commandPool)
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	vkAllocateCommandBuffers(device, &allocInfo, &m_CommandBuffer);
}

CommandBuffer::~CommandBuffer()
{
	vkFreeCommandBuffers(m_Device, m_CommandPool, 1, &m_CommandBuffer);
}

void CommandBuffer::Record()
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	if (vkBeginCommandBuffer(m_CommandBuffer, &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to begin recording command buffer!");
	}
}

void CommandBuffer::QueueSubmit(VkQueue queue, uint32_t count)
{
	vkEndCommandBuffer(m_CommandBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = count;
	submitInfo.pCommandBuffers = &m_CommandBuffer;

	vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(queue);
}
