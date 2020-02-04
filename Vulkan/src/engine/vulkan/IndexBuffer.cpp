#include "IndexBuffer.h"

#include "../Loader.h"
#include "Buffer.h"
#include "CommandBuffer.h"
#include "CommandPool.h"
#include "PhysicalDevice.h"
#include "Device.h"


IndexBuffer::IndexBuffer(PhysicalDevice* physicalDevice, Device* device, Loader* loader, Buffer* buffer, CommandPool* commandPool)
	: m_Device(device->m_Device)
{
	VkDeviceSize bufferSize = sizeof(loader->indices[0]) * loader->indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	Buffer* oStagingBuffer = new Buffer(physicalDevice, m_Device, bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	stagingBuffer = oStagingBuffer->m_Buffer;
	stagingBufferMemory = oStagingBuffer->m_Memory;

	void* data;
	vkMapMemory(m_Device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, loader->indices.data(), (size_t)bufferSize);
	vkUnmapMemory(m_Device, stagingBufferMemory);

	Buffer* oIndexBuffer = new Buffer(physicalDevice, m_Device, bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	m_Buffer = oIndexBuffer->m_Buffer;
	m_Memory = oIndexBuffer->m_Memory;

	copyBuffer(device, commandPool, stagingBuffer, m_Buffer, bufferSize);

	vkDestroyBuffer(m_Device, stagingBuffer, nullptr);
	vkFreeMemory(m_Device, stagingBufferMemory, nullptr);

	delete oIndexBuffer;
	delete oStagingBuffer;
}

IndexBuffer::~IndexBuffer()
{
	vkFreeMemory(m_Device, m_Memory, nullptr);
	vkDestroyBuffer(m_Device, m_Buffer, nullptr);
}

void IndexBuffer::copyBuffer(Device* device, CommandPool* commandPool,
	VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	CommandBuffer* commandBuffer = commandPool->beginSingleTimeCommands(device->m_Device);

	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer->m_CommandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	commandPool->endSingleTimeCommands(device->m_Device, commandBuffer, device->graphicsQueue);
}
