#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "Hazel/Renderer/RendererContext.h"

#include "VulkanAllocator.h"
#include "VulkanSwapChain.h"

struct GLFWwindow;

namespace Hazel
{
	// class VulkanContext : public GraphicsContext
	class VulkanContext : public RendererContext
	{
	public:
		VulkanContext(GLFWwindow* windowHandle);
		~VulkanContext();

		virtual void Create() override;
		virtual void SwapBuffers() override;

		virtual void OnResize(uint32_t width, uint32_t height) override;

		void Render();

		static VkInstance GetInstance() { return s_VulkanInstance; };

	private:
		void CreateDrawBuffers();
		void CreateDepthStencil();
		void CreateFramebuffer();

		void BuildCommandBuffer(uint32_t index, const glm::vec4& clearColor);
		void BuildTestCommandBuffer();

		bool m_Resized = false;

	private:
		GLFWwindow* m_WindowHandle;

		inline static VkInstance s_VulkanInstance;
		VkDebugReportCallbackEXT m_DebugReportCallback = VK_NULL_HANDLE;
		VkSurfaceKHR m_Surface;

		VulkanAllocator m_Allocator;
		VulkanSwapChain m_SwapChain;

		// TODO: Move to swap chain?
		VkFormat m_DepthBufferFormat;
		struct
		{
			VkImage Image;
			VkDeviceMemory DeviceMemory;
			VkImageView ImageView;
		} m_DepthStencil;

		struct
		{
			// Swap chain
			VkSemaphore PresentComplete;
			// Command buffer
			VkSemaphore RenderComplete;

		} m_Semaphores;

		uint32_t m_CurrentBufferIndex;

		std::vector<VkCommandBuffer> m_DrawCommandBuffers;

		Ref<VulkanDevice> m_Device;

	};

}
