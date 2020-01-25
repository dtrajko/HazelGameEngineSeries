#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class DescriptorSetLayout;


class PipelineLayout
{

public:
	
	VkPipelineLayout m_PipelineLayout;
	VkDevice m_device;


public:

	PipelineLayout(VkDevice device, DescriptorSetLayout* descriptorSetLayout);
	~PipelineLayout();

};
