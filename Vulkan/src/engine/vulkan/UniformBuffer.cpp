#include "UniformBuffer.h"

#include "SwapChain.h"
#include "Buffer.h"
#include "PhysicalDevice.h"


void UniformBuffer::createUniformBuffers(PhysicalDevice* physicalDevice, VkDevice device, SwapChain* swapChain)
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(swapChain->swapChainImages.size());
	uniformBuffersMemory.resize(swapChain->swapChainImages.size());

	for (size_t i = 0; i < swapChain->swapChainImages.size(); i++)
	{
		Buffer* buffer = new Buffer(physicalDevice, device, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		uniformBuffers[i] = buffer->m_Buffer;
		uniformBuffersMemory[i] = buffer->m_Memory;
	}
}
