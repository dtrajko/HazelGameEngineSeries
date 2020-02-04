#include "VulkanApp.h"

#include "Hazel/Window.h"
#include "Hazel/Input.h"
#include "Hazel/Core.h"
#include "engine/Print.h"


#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)


VulkanLayer::VulkanLayer()
	: Layer("VulkanLayer"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f, 0.0f, 10.0f)
{
	window = &(Hazel::Application::Get().GetWindow());
	windowHandler = (GLFWwindow*)window->GetNativeWindow();

	InitVulkan();
}

void VulkanLayer::OnUpdate(Hazel::Timestep timestep)
{
	// HZ_TRACE("Delta time: {0} sec, {1} ms", timestep.GetSeconds(), timestep.GetMilliseconds());

	m_FPS = (unsigned int)(1.0f / timestep.GetSeconds());

	UpdateInputPolling(timestep);

	m_Camera.SetPosition(m_CameraPosition);
	m_Camera.SetRotation(m_CameraRotation);

	DrawFrame(device);
	vkDeviceWaitIdle(device->m_Device);
}

void VulkanLayer::UpdateInputPolling(Hazel::Timestep timestep)
{
	if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT) || Hazel::Input::IsKeyPressed(HZ_KEY_A))
	{
		m_CameraPosition.x -= m_CameraMoveSpeed * timestep.GetSeconds();
	}
	else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT) || Hazel::Input::IsKeyPressed(HZ_KEY_D))
	{
		m_CameraPosition.x += m_CameraMoveSpeed * timestep.GetSeconds();
	}

	if (Hazel::Input::IsKeyPressed(HZ_KEY_UP) || Hazel::Input::IsKeyPressed(HZ_KEY_W))
	{
		m_CameraPosition.z -= m_CameraMoveSpeed * timestep.GetSeconds();
	}
	else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN) || Hazel::Input::IsKeyPressed(HZ_KEY_S))
	{
		m_CameraPosition.z += m_CameraMoveSpeed * timestep.GetSeconds();
	}

	if (Hazel::Input::IsKeyPressed(HZ_KEY_Q))
	{
		m_CameraPosition.y += m_CameraMoveSpeed * timestep.GetSeconds();
	}
	else if (Hazel::Input::IsKeyPressed(HZ_KEY_E))
	{
		m_CameraPosition.y -= m_CameraMoveSpeed * timestep.GetSeconds();
	}

	if (Hazel::Input::IsKeyPressed(HZ_KEY_1))
	{
		m_CameraRotation += m_CameraRotationSpeed * timestep.GetSeconds();
	}
	else if (Hazel::Input::IsKeyPressed(HZ_KEY_2))
	{
		m_CameraRotation -= m_CameraRotationSpeed * timestep.GetSeconds();
	}
}

void VulkanLayer::OnEvent(Hazel::Event& event)
{
	Hazel::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Hazel::WindowResizeEvent>(HZ_BIND_EVENT_FN(VulkanLayer::OnWindowResizeEvent));
}

bool VulkanLayer::OnWindowResizeEvent(Hazel::WindowResizeEvent& event)
{
	RecreateSwapChain();

	return false;
}

VulkanLayer::~VulkanLayer()
{
	Cleanup();
}


void VulkanLayer::InitVulkan()
{
	instance = new Instance(enableValidationLayers, validationLayers, validationLayer);
	debug = new Debug(instance->hInstance, enableValidationLayers);
	surface = new Surface(instance->hInstance, windowHandler);
	loader = new Loader();
	imageFactory = new ImageFactory();
	physicalDevice = new PhysicalDevice(instance->hInstance, surface->m_surfaceKHR, imageFactory->msaaSamples);
	PrintDevicePropertiesBasic(physicalDevice->m_Device);
	device = new Device(physicalDevice, surface->m_surfaceKHR, enableValidationLayers);
	swapChain = new SwapChain(windowHandler, physicalDevice, device->m_Device, surface);
	swapChain->createImageViews(device->m_Device);
	renderPass = new RenderPass(physicalDevice, device->m_Device, swapChain, imageFactory);
	descriptorSetLayout = new DescriptorSetLayout(device->m_Device);
	graphicsPipeline = new GraphicsPipeline(device->m_Device, swapChain, imageFactory, descriptorSetLayout, renderPass);
	commandPool = new CommandPool(physicalDevice, device->m_Device, surface->m_surfaceKHR);
	imageFactory->createColorResources(device->m_Device, physicalDevice, swapChain);
	imageFactory->createDepthResources(device, physicalDevice, swapChain, commandPool, format);
	framebuffer.createFramebuffers(device->m_Device, swapChain, imageFactory, renderPass->m_RenderPass);
	imageFactory->createTextureImage(loader->TEXTURE_PATH.c_str(), device, physicalDevice, commandPool, format);
	imageFactory->createTextureImageView(device->m_Device);
	textureSampler = new Sampler(device->m_Device, imageFactory->mipLevels);
	loader->loadModel();
	vertexBuffer = new VertexBuffer(physicalDevice, device, loader, indexBuffer, commandPool);
	indexBuffer = new IndexBuffer(physicalDevice, device, loader, buffer, commandPool);
	uniformBuffer.createUniformBuffers(physicalDevice, device->m_Device, swapChain);
	descriptorPool = new DescriptorPool(device->m_Device, swapChain);
	descriptorSet.createDescriptorSets(device->m_Device, uniformBuffer, swapChain, descriptorSetLayout, descriptorPool,
		imageFactory->imageTexture->m_ImageView->m_ImageView, textureSampler);
	commandPool->createCommandBuffers(device->m_Device, loader, renderPass->m_RenderPass, swapChain, framebuffer.swapChainFramebuffers,
		graphicsPipeline->m_Pipeline, graphicsPipeline->m_PipelineLayout->m_PipelineLayout, vertexBuffer, indexBuffer, descriptorSet);
	CreateSyncObjects();
}

void VulkanLayer::PrintDevicePropertiesBasic(VkPhysicalDevice physicalDevice)
{
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

	HZ_CORE_INFO("Vulkan Info:");
	HZ_CORE_INFO("   Vendor: {0}", deviceProperties.vendorID);
	HZ_CORE_INFO("   Renderer: {0}", deviceProperties.deviceName);
	HZ_CORE_INFO("   Version: {0}", deviceProperties.apiVersion);
}

void VulkanLayer::UpdateUniformBuffer(uint32_t currentImage, UniformBuffer uniformBuffer)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo = {};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ubo.model = glm::rotate(ubo.model, time * glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	// ubo.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f + positionZ), glm::vec3(0.0f + positionX, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ubo.view = m_Camera.GetViewMatrix();
	ubo.proj = glm::perspective(glm::radians(45.0f), (float)swapChain->swapChainExtent.width / (float)swapChain->swapChainExtent.height, 0.1f, 20.0f);
	ubo.proj[1][1] *= -1;

	void* data;
	vkMapMemory(device->m_Device, uniformBuffer.uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(device->m_Device, uniformBuffer.uniformBuffersMemory[currentImage]);
}

void VulkanLayer::CreateSyncObjects()
{
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	imagesInFlight.resize(swapChain->swapChainImages.size(), VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(device->m_Device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create a VkSemaphore ('imageAvailableSemaphore')!");
		}

		if (vkCreateSemaphore(device->m_Device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create a VkSemaphore ('renderFinishedSemaphore')!");
		}

		if (vkCreateFence(device->m_Device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create a VkFence object!");
		}
	}
}

void VulkanLayer::DrawFrame(Device* device)
{
	vkWaitForFences(device->m_Device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	// Acquiring an image from the swap chain
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(device->m_Device, swapChain->swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("Failed to acquire swap chain image!");
	}

	// Update uniform buffer
	UpdateUniformBuffer(imageIndex, uniformBuffer);

	// Check if a previous frame is using this image (i.e. there is its fence to wait on)
	if (imagesInFlight[imageIndex] != VK_NULL_HANDLE)
	{
		vkWaitForFences(device->m_Device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}

	// Mark the image as now being in use by this frame
	imagesInFlight[imageIndex] = inFlightFences[currentFrame];

	// Submitting the command buffer
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandPool->commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(device->m_Device, 1, &inFlightFences[currentFrame]);

	if (vkQueueSubmit(device->graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapChain->swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	result = vkQueuePresentKHR(device->presentQueue, &presentInfo);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window_framebufferResized)
	{
		window_framebufferResized = false;
		RecreateSwapChain();
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanLayer::CleanupSwapChain(UniformBuffer uniformBuffer)
{
	// imageFactory->cleanUp(device->m_Device);

	for (auto framebuffer : framebuffer.swapChainFramebuffers)
	{
		vkDestroyFramebuffer(device->m_Device, framebuffer, nullptr);
	}

	vkFreeCommandBuffers(device->m_Device, commandPool->commandPool, static_cast<uint32_t>(commandPool->commandBuffers.size()), commandPool->commandBuffers.data());

	graphicsPipeline->cleanUp();
	renderPass->cleanUp();

	for (auto imageView : swapChain->swapChainImageViews)
	{
		vkDestroyImageView(device->m_Device, imageView, nullptr);
	}

	vkDestroySwapchainKHR(device->m_Device, swapChain->swapChain, nullptr);

	for (size_t i = 0; i < swapChain->swapChainImages.size(); i++)
	{
		vkDestroyBuffer(device->m_Device, uniformBuffer.uniformBuffers[i], nullptr);
		vkFreeMemory(device->m_Device, uniformBuffer.uniformBuffersMemory[i], nullptr);
	}

	vkDestroyDescriptorPool(device->m_Device, descriptorPool->m_DescriptorPool, nullptr);
}

void VulkanLayer::RecreateSwapChain()
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(windowHandler, &width, &height);
	while (width == 0 || height == 0)
	{
		glfwGetFramebufferSize(windowHandler, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(device->m_Device);

	CleanupSwapChain(uniformBuffer);

	swapChain->createSwapChain(windowHandler, physicalDevice, device->m_Device, surface);
	swapChain->createImageViews(device->m_Device);
	renderPass->createRenderPass(physicalDevice, device->m_Device, swapChain, imageFactory);
	graphicsPipeline->createGraphicsPipeline(device->m_Device, swapChain, imageFactory, descriptorSetLayout, renderPass);
	imageFactory->createColorResources(device->m_Device, physicalDevice, swapChain);
	imageFactory->createDepthResources(device, physicalDevice, swapChain, commandPool, format);
	framebuffer.createFramebuffers(device->m_Device, swapChain, imageFactory, renderPass->m_RenderPass);
	uniformBuffer.createUniformBuffers(physicalDevice, device->m_Device, swapChain);
	descriptorPool->createDescriptorPool(device->m_Device, swapChain);
	descriptorSet.createDescriptorSets(device->m_Device, uniformBuffer, swapChain, descriptorSetLayout, descriptorPool,
		imageFactory->imageTexture->m_ImageView->m_ImageView, textureSampler);
	commandPool->createCommandBuffers(device->m_Device, loader, renderPass->m_RenderPass, swapChain, framebuffer.swapChainFramebuffers,
		graphicsPipeline->m_Pipeline, graphicsPipeline->m_PipelineLayout->m_PipelineLayout, vertexBuffer, indexBuffer, descriptorSet);
}

void VulkanLayer::Cleanup()
{
	delete imageFactory;

	CleanupSwapChain(uniformBuffer);

	delete textureSampler;
	delete descriptorSetLayout;
	delete indexBuffer;
	delete vertexBuffer;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(device->m_Device, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(device->m_Device, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(device->m_Device, inFlightFences[i], nullptr);
	}

	delete commandPool;
	delete graphicsPipeline;
	delete renderPass;
	delete device;
	delete loader;
	delete surface;
	delete debug;
	delete instance;

	glfwDestroyWindow(windowHandler);

	glfwTerminate();
}


VulkanApp::VulkanApp()
{
	PushLayer(new VulkanLayer());
	// PushOverlay(new Hazel::ImGuiLayer());
}

VulkanApp::~VulkanApp()
{
}
