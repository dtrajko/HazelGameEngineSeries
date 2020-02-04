#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class ShaderModule;
class SwapChain;
class ImageFactory;
class DescriptorSetLayout;
class RenderPass;
class PipelineLayout;


class GraphicsPipeline
{

public:

	// Graphics pipeline
	VkPipeline m_Pipeline;
	VkDevice m_Device;
	PipelineLayout* m_PipelineLayout;
	ShaderModule* m_VertShaderModule;
	ShaderModule* m_FragShaderModule;

public:

	GraphicsPipeline(VkDevice device, SwapChain* swapChain, ImageFactory* imageFactory,
		DescriptorSetLayout* descriptorSetLayout, RenderPass* renderPass);

	void createGraphicsPipeline(VkDevice device, SwapChain* swapChain, ImageFactory* imageFactory,
		DescriptorSetLayout* descriptorSetLayout, RenderPass* renderPass);
	void cleanUp();
	~GraphicsPipeline();

};
