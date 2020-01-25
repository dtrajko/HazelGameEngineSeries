#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class Sampler
{

public:

	VkSampler m_Sampler;
	VkDevice m_Device;

	Sampler(VkDevice device, uint32_t mipLevels);
	~Sampler();

};
