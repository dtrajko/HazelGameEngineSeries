#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanDevice.h"
#include "VulkanAllocator.h"

struct GLFWwindow;

namespace Hazel {

	typedef struct _SwapChainBuffers {
		VkImage image;
		VkImageView view;
	} SwapChainBuffer;

	class VulkanSwapChain {
	public:
		VulkanSwapChain() = default;

		void Init(VkInstance instance, const Ref<VulkanDevice>& device);
		void InitSurface(GLFWwindow* windowHandle);
		void Create(uint32_t* width, uint32_t* height, bool vsync = false);

		void OnResize(uint32_t width, uint32_t height);
		void Present();

		VkResult AcquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex);
		VkResult QueuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore);

		void Cleanup();

		uint32_t GetImageCount() const { return m_ImageCount; }

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

		VkRenderPass GetRenderPass() { return m_RenderPass; }
		VkFramebuffer GetFramebuffer(uint32_t index) {
			HZ_CORE_ASSERT(index < m_ImageCount, "");
			return m_Framebuffers[index];
		}
		VkCommandBuffer GetDrawCommandBuffer(uint32_t index)
		{
			HZ_CORE_ASSERT(index < m_ImageCount, "");
			return m_DrawCommandBuffers[index];
		}
	private:
		void CreateFramebuffer();
		void CreateDepthStencil();
		void CreateDrawBuffers();
		void FindImageFormatAndColorSpace();

	private:
		VkInstance m_Instance;
		Ref<VulkanDevice> m_Device;
		Ref<VulkanAllocator> m_Allocator;

		VkFormat m_ColorFormat;
		VkColorSpaceKHR m_ColorSpace;

	private:
		uint32_t m_ImageCount;
		uint32_t m_Width;
		uint32_t m_Height;

		VkRenderPass m_RenderPass;
		VkFramebuffer m_Framebuffer;

		std::vector<VkFramebuffer> m_Framebuffers;
		std::vector<VkCommandBuffer> m_DrawCommandBuffers;

	};

}
