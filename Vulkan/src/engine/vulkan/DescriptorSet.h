#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>


class UniformBuffer;
class SwapChain;
class DescriptorSetLayout;
class DescriptorPool;
class Image;
class Sampler;


class DescriptorSet
{
public:

	// Descriptors
	std::vector<VkDescriptorSet> descriptorSets;

	void createDescriptorSets(VkDevice device, UniformBuffer uniformBuffer, SwapChain* swapChain,
		DescriptorSetLayout* descriptorSetLayout, DescriptorPool* descriptorPool, VkImageView imageViewTexture, Sampler* sampler);


};
