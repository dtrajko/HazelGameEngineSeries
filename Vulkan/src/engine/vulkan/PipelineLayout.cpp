#include "PipelineLayout.h"

#include "DescriptorSetLayout.h"

#include <stdexcept>


PipelineLayout::PipelineLayout(VkDevice device, DescriptorSetLayout* descriptorSetLayout) : m_device(device)
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout->m_DescriptorSetLayout;

	pipelineLayoutInfo.pushConstantRangeCount = 0;

	if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create pipeline layout");
	}
}

PipelineLayout::~PipelineLayout()
{
	vkDestroyPipelineLayout(m_device, m_PipelineLayout, nullptr);
}
