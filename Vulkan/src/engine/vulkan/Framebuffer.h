#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>


class SwapChain;
class ImageFactory;


class Framebuffer
{
public:

	std::vector<VkFramebuffer> swapChainFramebuffers;

public:

	void createFramebuffers(VkDevice device, SwapChain* swapChain, ImageFactory* imageFactory, VkRenderPass renderPass);

};
