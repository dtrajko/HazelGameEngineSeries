#include "hzpch.h"

#include "VulkanContext.h"
#include "Hazel/Renderer/RendererAPI.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>


namespace Hazel
{
	VulkanContext::VulkanContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(windowHandle, "Window handle is null!");

		RendererAPI::SetAPI(RendererAPI::API::Vulkan);
	}

	VulkanContext::~VulkanContext()
	{
	}

	void VulkanContext::Create()
	{
		// glfwMakeContextCurrent(m_WindowHandle);
		// int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		// HZ_CORE_ASSERT(status, "Failed to initialize Glad!");
		// 
		// HZ_CORE_INFO("OpenGL Info:");
		// HZ_CORE_INFO("   Vendor: {0}", glGetString(GL_VENDOR));
		// HZ_CORE_INFO("   Renderer: {0}", glGetString(GL_RENDERER));
		// HZ_CORE_INFO("   Version: {0}", glGetString(GL_VERSION));
	}

	void VulkanContext::SwapBuffers()
	{
		// glfwSwapBuffers(m_WindowHandle);
		// Update command buffer
		BuildCommandBuffer(m_CurrentBufferIndex, glm::vec4());

		// ...

		Render();
	}

	void VulkanContext::Render()
	{
		const uint64_t DEFAULT_FENCE_TIMEOUT = 100000000000;

		// Get next image in the swap chain (back/front buffer)
		VK_CHECK_RESULT(m_SwapChain.AcquireNextImage(m_Semaphores.PresentComplete, &m_CurrentBufferIndex));

		// Use a fence to wait until the command buffer has finished execution before using it again
		VK_CHECK_RESULT(vkWaitForFences(m_Device->GetVulkanDevice(), 1, &m_WaitFences[m_CurrentBufferIndex], VK_TRUE, UINT64_MAX));
		VK_CHECK_RESULT(vkResetFences(m_Device->GetVulkanDevice(), 1, &m_WaitFences[m_CurrentBufferIndex]));

		// Pipeline stage at which the queue submission will wait (via pWaitSemaphores)
		VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		// The submit info structure specifies a command buffer queue submission batch
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pWaitDstStageMask = &waitStageMask;
		submitInfo.pWaitSemaphores = &m_Semaphores.PresentComplete;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &m_Semaphores.RenderComplete;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pCommandBuffers = &m_DrawCommandBuffers[m_CurrentBufferIndex];
		submitInfo.commandBufferCount = 1;

		// Submit to the graphics queue passing a wait fence
		VK_CHECK_RESULT(vkQueueSubmit(m_Device->GetQueue(), 1, &submitInfo, m_WaitFences[m_CurrentBufferIndex]));

		// Present the current buffer to the swap chain
		// Pass the semaphore signaled by the command buffer submission from the submit info as the wait semaphore for swap chain presentation
		// This ensures that the image is not presented to the windowing system until all commands have been submitted
		VkResult result = m_SwapChain.QueuePresent(m_Device->GetQueue(), m_CurrentBufferIndex, m_Semaphores.RenderComplete);

		if (result != VK_SUCCESS || result == VK_SUBOPTIMAL_KHR)
		{
			if (result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				// Swap chain is no longer compatible with the surface and needs to be recreated
			}
		}
	}

	void VulkanContext::OnResize(uint32_t width, uint32_t height)
	{
	}

	void VulkanContext::CreateDrawBuffers()
	{
	}
	void VulkanContext::CreateDepthStencil()
	{
	}
	void VulkanContext::CreateFramebuffer()
	{
	}
	void VulkanContext::BuildCommandBuffer(uint32_t index, const glm::vec4& clearColor)
	{
	}
	void VulkanContext::BuildTestCommandBuffer()
	{
	}
}
