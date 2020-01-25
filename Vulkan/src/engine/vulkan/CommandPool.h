#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "PhysicalDevice.h"

#include <vector>

class Loader;
class SwapChain;
class VertexBuffer;
class IndexBuffer;
class DescriptorSet;
class CommandBuffer;


class CommandPool
{

public:

	// Command pool
	VkCommandPool commandPool;
	VkDevice m_device;

	// Command buffers
	std::vector<VkCommandBuffer> commandBuffers;

public:

	CommandPool(PhysicalDevice* physicalDevice, VkDevice device, VkSurfaceKHR surfaceKHR);
	~CommandPool();

	void createCommandBuffers(VkDevice device, Loader* loader, VkRenderPass renderPass, SwapChain* swapChain,
		std::vector<VkFramebuffer> swapChainFramebuffers, VkPipeline graphicsPipeline, VkPipelineLayout pipelineLayout,
		VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, DescriptorSet descriptorSet);

	// From CommandBuffer
	CommandBuffer* beginSingleTimeCommands(VkDevice device);

	void endSingleTimeCommands(VkDevice device, CommandBuffer* commandBuffer, VkQueue graphicsQueue);

	void copyBufferToImage(VkDevice device, VkQueue graphicsQueue, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);


};
