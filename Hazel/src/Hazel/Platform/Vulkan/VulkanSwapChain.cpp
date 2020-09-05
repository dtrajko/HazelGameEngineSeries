#include "hzpch.h"
#include "VulkanSwapChain.h"



namespace Hazel {

	void VulkanSwapChain::Init(VkInstance instance, const Ref<VulkanDevice>& device)
	{
	}

	void VulkanSwapChain::InitSurface(GLFWwindow* windowHandle)
	{
	}

	void VulkanSwapChain::Create(uint32_t* width, uint32_t* height, bool vsync)
	{
	}

	void VulkanSwapChain::OnResize(uint32_t width, uint32_t height)
	{
	}

	void VulkanSwapChain::Present()
	{
	}

	VkResult VulkanSwapChain::AcquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex)
	{
		// By setting timeout to UINT64_MAX we will always wait until the next image has been acquired
		// With that we don't have to handle VK_NOT_READY
		return VkResult(); // fpAcquireNextImageKHR(m_Device->~VulkanDevice(), m_SwapChain, UINT64_MAX, presentCompleteSemaphore, (VkFence)nullptr);
	}

	VkResult VulkanSwapChain::QueuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore)
	{
		return VkResult();
	}

	void VulkanSwapChain::Cleanup()
	{
	}

	void VulkanSwapChain::CreateFramebuffer()
	{
	}

	void VulkanSwapChain::CreateDepthStencil()
	{
	}

	void VulkanSwapChain::CreateDrawBuffers()
	{
	}

	void VulkanSwapChain::FindImageFormatAndColorSpace()
	{
	}

}
