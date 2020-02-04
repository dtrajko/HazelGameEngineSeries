#include "VertexBuffer.h"

#include "../Loader.h"
#include "IndexBuffer.h"
#include "CommandBuffer.h"
#include "CommandPool.h"
#include "Buffer.h"
#include "PhysicalDevice.h"
#include "Device.h"


VertexBuffer::VertexBuffer(PhysicalDevice* physicalDevice, Device* device, Loader* loader, IndexBuffer* indexBuffer, CommandPool* commandPool)
	: m_Device(device->m_Device)
{
	VkDeviceSize bufferSize = sizeof(loader->vertices[0]) * loader->vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	Buffer* oStagingBuffer = new Buffer(physicalDevice, m_Device, bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	stagingBuffer = oStagingBuffer->m_Buffer;
	stagingBufferMemory = oStagingBuffer->m_Memory;

	void* data;
	vkMapMemory(m_Device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, loader->vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(m_Device, stagingBufferMemory);

	Buffer* oVertexBuffer = new Buffer(physicalDevice, m_Device, bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	m_Buffer = oVertexBuffer->m_Buffer;
	m_Memory = oVertexBuffer->m_Memory;

	indexBuffer->copyBuffer(device, commandPool, stagingBuffer, m_Buffer, bufferSize);

	vkDestroyBuffer(m_Device, stagingBuffer, nullptr);
	vkFreeMemory(m_Device, stagingBufferMemory, nullptr);

	delete oVertexBuffer;
	delete oStagingBuffer;
}

VertexBuffer::~VertexBuffer()
{
	vkDestroyBuffer(m_Device, m_Buffer, nullptr);
	vkFreeMemory(m_Device, m_Memory, nullptr);
}
