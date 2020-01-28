#include "hzpch.h"

#include "VulkanContext.h"
#include "Hazel/Application.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>


namespace Hazel
{
	VulkanContext::VulkanContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(windowHandle, "Window handle is null!");

		Application::s_Graphics_API = GraphicsAPI::VULKAN;
	}

	void VulkanContext::Init()
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
	}
}
