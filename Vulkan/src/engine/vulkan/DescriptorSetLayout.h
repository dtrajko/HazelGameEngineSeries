#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class DescriptorSetLayout
{

public:

	VkDescriptorSetLayout m_DescriptorSetLayout;
	VkDevice m_Device;

public:

	DescriptorSetLayout(VkDevice device);
	~DescriptorSetLayout();

};
