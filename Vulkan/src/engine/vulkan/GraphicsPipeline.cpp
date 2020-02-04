#include "GraphicsPipeline.h"

#include "../Loader.h"
#include "ShaderModule.h"
#include "SwapChain.h"
#include "ImageFactory.h"
#include "ShaderModule.h"
#include "DescriptorSetLayout.h"
#include "RenderPass.h"
#include "PipelineLayout.h"

#include <iostream>


GraphicsPipeline::GraphicsPipeline(VkDevice device, SwapChain* swapChain, ImageFactory* imageFactory,
	DescriptorSetLayout* descriptorSetLayout, RenderPass* renderPass) : m_Device(device)
{
	createGraphicsPipeline(device, swapChain, imageFactory, descriptorSetLayout, renderPass);
}

void GraphicsPipeline::createGraphicsPipeline(VkDevice device, SwapChain* swapChain, ImageFactory* imageFactory,
	DescriptorSetLayout* descriptorSetLayout, RenderPass* renderPass)
{
	auto vertShaderCode = Loader::readFile("shaders/shader_vertex.spv");
	auto fragShaderCode = Loader::readFile("shaders/shader_fragment.spv");

	m_VertShaderModule = new ShaderModule(device, vertShaderCode);
	m_FragShaderModule = new ShaderModule(device, fragShaderCode);

	std::cout << std::endl;
	std::cout << "Loading vertex and fragment shaders in binary SPIR-V format:" << std::endl;
	std::cout << "\t" << "Vertex shader size:" << vertShaderCode.size() << std::endl;
	std::cout << "\t" << "Fragment shader size:" << fragShaderCode.size() << std::endl;

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = m_VertShaderModule->m_ShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = m_FragShaderModule->m_ShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	// Fixed functions - Vertex input
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; // Optional
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data(); // Optional

	// Fixed functions - Input assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	// Fixed functions - Viewports and scissors
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChain->swapChainExtent.width;
	viewport.height = (float)swapChain->swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChain->swapChainExtent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	// Fixed functions - Rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	// Fixed functions - Multisampling
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_TRUE; // enable sample shading in the pipeline
	multisampling.minSampleShading = 0.2f; // min fraction for sample shading; closer to one is smoother
	multisampling.rasterizationSamples = imageFactory->msaaSamples;

	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	// Fixed functions - Color blending
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	// Fixed functions - Pipeline layout
	m_PipelineLayout = new PipelineLayout(m_Device, descriptorSetLayout);

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	// shader-stage
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	// fixed-function stage
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	// pipeline layout
	pipelineInfo.layout = m_PipelineLayout->m_PipelineLayout;
	// render pass
	pipelineInfo.renderPass = renderPass->m_RenderPass;
	pipelineInfo.subpass = 0;

	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional

	if (vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create the Graphics pipeline!");
	}

	vkDestroyShaderModule(m_Device, m_FragShaderModule->m_ShaderModule, nullptr);
	vkDestroyShaderModule(m_Device, m_VertShaderModule->m_ShaderModule, nullptr);
}

void GraphicsPipeline::cleanUp()
{
	vkDestroyPipeline(m_Device, m_Pipeline, nullptr);
}

GraphicsPipeline::~GraphicsPipeline()
{
	delete m_VertShaderModule;
	delete m_FragShaderModule;
	delete m_PipelineLayout;
}
