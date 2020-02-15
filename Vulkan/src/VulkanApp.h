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

#include "Hazel/Core/Window.h"
#include "Hazel/Renderer/RendererAPI.h"
#include "Hazel/Renderer/CameraController.h"


const int MAX_FRAMES_IN_FLIGHT = 2;

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = false;
#endif // NDEBUG


class VulkanLayer : public Hazel::Layer
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

	// Model movement
	float positionX = 0.0f;
	float positionZ = 0.0f;
	float movementSpeed = 0.02f;

	unsigned int m_FPS;

	Hazel::CameraController m_CameraController;


public:

	VulkanLayer();
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(Hazel::Timestep timestep) override;
	void OnEvent(Hazel::Event& event) override;
	~VulkanLayer();

private:

	void InitVulkan();

	void PrintDevicePropertiesBasic(VkPhysicalDevice physicalDevice);

	// Swap chain support
	void CleanupSwapChain(UniformBuffer uniformBuffer);
	void RecreateSwapChain();

	// Semaphores (for synchronizing swap chain events)
	void CreateSyncObjects();

	// Uniform buffers
	void UpdateUniformBuffer(uint32_t currentImage, UniformBuffer uniformBuffer);

	bool OnWindowResizeEvent(Hazel::WindowResizeEvent& event);

	void DrawFrame(Device* device);

	void Cleanup();

};


class VulkanApp : public Hazel::Application
{

public:

	VulkanApp();
	~VulkanApp();

};


Hazel::Application* Hazel::CreateApplication()
{
	RendererAPI::SetAPI(RendererAPI::API::Vulkan);

	return new VulkanApp();
};

