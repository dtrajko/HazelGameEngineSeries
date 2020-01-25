#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class SwapChain;


class DescriptorPool
{
public:

	VkDevice m_Device;
	VkDescriptorPool m_DescriptorPool;

public:

	DescriptorPool(VkDevice device, SwapChain* swapChain);

	void createDescriptorPool(VkDevice device, SwapChain* swapChain);

	~DescriptorPool();

};
