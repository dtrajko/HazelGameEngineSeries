#include "IndexBuffer.h"

#include "../Loader.h"
#include "Buffer.h"
#include "CommandBuffer.h"
#include "CommandPool.h"
#include "PhysicalDevice.h"


IndexBuffer::IndexBuffer(PhysicalDevice* physicalDevice, VkDevice device, Loader* loader, Buffer* buffer,
	VkQueue graphicsQueue, CommandPool* commandPool) : m_Device(device)
{
	VkDeviceSize bufferSize = sizeof(loader->indices[0]) * loader->indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	Buffer* oStagingBuffer = new Buffer(physicalDevice, device, bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	stagingBuffer = oStagingBuffer->m_Buffer;
	stagingBufferMemory = oStagingBuffer->m_Memory;

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, loader->indices.data(), (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	Buffer* oIndexBuffer = new Buffer(physicalDevice, device, bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	m_Buffer = oIndexBuffer->m_Buffer;
	m_Memory = oIndexBuffer->m_Memory;

	copyBuffer(device, graphicsQueue, commandPool, stagingBuffer, m_Buffer, bufferSize);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);

	delete oIndexBuffer;
	delete oStagingBuffer;
}

IndexBuffer::~IndexBuffer()
{
	vkFreeMemory(m_Device, m_Memory, nullptr);
	vkDestroyBuffer(m_Device, m_Buffer, nullptr);
}

void IndexBuffer::copyBuffer(VkDevice device, VkQueue graphicsQueue, CommandPool* commandPool,
	VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	CommandBuffer* commandBuffer = commandPool->beginSingleTimeCommands(device);

	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer->m_CommandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	commandPool->endSingleTimeCommands(device, commandBuffer, graphicsQueue);
}
