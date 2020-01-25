#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class PhysicalDevice;
class SwapChain;
class ImageFactory;


class RenderPass
{

public:

	// Render pass
	VkRenderPass m_RenderPass;
	VkDevice m_Device;


public:

	RenderPass(PhysicalDevice* physicalDevice, VkDevice device, SwapChain* swapChain, ImageFactory* imageFactory);

	void createRenderPass(PhysicalDevice* physicalDevice, VkDevice device, SwapChain* swapChain, ImageFactory* imageFactory);

	void cleanUp();

	~RenderPass();

};
