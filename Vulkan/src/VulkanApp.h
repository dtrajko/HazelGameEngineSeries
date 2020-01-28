#include <Hazel.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <iostream>
#include <chrono>

#include "engine/Debug.h"
#include "engine/Loader.h"
#include "engine/Input.h"
#include "engine/vulkan/PhysicalDevice.h"
#include "engine/vulkan/ValidationLayer.h"
#include "engine/vulkan/Surface.h"
#include "engine/vulkan/Device.h"
#include "engine/vulkan/Buffer.h"
#include "engine/vulkan/VertexBuffer.h"
#include "engine/vulkan/IndexBuffer.h"
#include "engine/vulkan/UniformBuffer.h"
#include "engine/vulkan/CommandBuffer.h"
#include "engine/vulkan/CommandPool.h"
#include "engine/vulkan/Image.h"
#include "engine/vulkan/ImageFactory.h"
#include "engine/vulkan/ImageView.h"
#include "engine/vulkan/Format.h"
#include "engine/vulkan/Sampler.h"
#include "engine/vulkan/ShaderModule.h"
#include "engine/vulkan/DescriptorSetLayout.h"
#include "engine/vulkan/DescriptorPool.h"
#include "engine/vulkan/DescriptorSet.h"
#include "engine/vulkan/SwapChain.h"
#include "engine/vulkan/Framebuffer.h"
#include "engine/vulkan/PipelineLayout.h"
#include "engine/vulkan/Instance.h"
#include "engine/vulkan/RenderPass.h"
#include "engine/vulkan/GraphicsPipeline.h"

#include "Hazel/Window.h"

#define GRAPHICS_API_VULKAN


const int MAX_FRAMES_IN_FLIGHT = 2;

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = false;
#endif // NDEBUG


class VulkanLayer : public Hazel::Layer
{

public:

	VulkanLayer() : Layer("VulkanLayer")
	{
	
	}

	void OnUpdate() override
	{
		// HZ_INFO("ExampleLayer::Update");
	}

	void OnEvent(Hazel::Event& event) override
	{
		HZ_TRACE("{0}", event);
	}

};

class VulkanApp : public Hazel::Application
{

private:

	// Semaphores (for synchronizing swap chain events)
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

	// temporary variable
	bool window_framebufferResized = false;

	// Refactoring
	ValidationLayer validationLayer;
	ShaderModule shaderModule;
	DescriptorSet descriptorSet;
	Framebuffer framebuffer;
	Format format;

	// RAII
	Debug* debug;
	Instance* instance;
	Hazel::Window* window;
	GLFWwindow* windowHandler;
	Surface* surface;
	Loader* loader;
	PhysicalDevice* physicalDevice;
	Device* device;
	Buffer* buffer;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	UniformBuffer uniformBuffer;
	Image* image;
	ImageFactory* imageFactory;
	Sampler* textureSampler;
	CommandPool* commandPool;
	DescriptorSetLayout* descriptorSetLayout;
	DescriptorPool* descriptorPool;
	SwapChain* swapChain;
	RenderPass* renderPass;
	GraphicsPipeline* graphicsPipeline;

	// movement
	float positionX = 0.0f;
	float positionZ = 0.0f;


public:

	VulkanApp();
	void Run();
	~VulkanApp();

private:

	void initVulkan();

	void printDevicePropertiesBasic(VkPhysicalDevice physicalDevice);

	// Swap chain support
	void cleanupSwapChain(UniformBuffer uniformBuffer);
	void recreateSwapChain();

	// Semaphores (for synchronizing swap chain events)
	void createSyncObjects();

	// Uniform buffers
	void updateUniformBuffer(uint32_t currentImage, UniformBuffer uniformBuffer);

	void drawFrame(Device* device);

	void cleanup();

};

Hazel::Application* Hazel::CreateApplication()
{
	Application::s_Graphics_API = GraphicsAPI::VULKAN;

	return new VulkanApp();
}
