#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "PhysicalDevice.h"
#include "Surface.h"


class ImageView;


class SwapChain
{
public:

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;

public:

	SwapChain(GLFWwindow* window, PhysicalDevice* physicalDevice, VkDevice device, Surface* surface);
	~SwapChain();

	void createSwapChain(GLFWwindow* window, PhysicalDevice* physicalDevice, VkDevice device, Surface* surface);

	void createImageViews(VkDevice device);

};
