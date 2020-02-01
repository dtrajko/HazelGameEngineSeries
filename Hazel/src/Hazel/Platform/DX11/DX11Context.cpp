#include "hzpch.h"

#include "DX11Context.h"
#include "Hazel/Renderer/RendererAPI.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>


namespace Hazel
{
	DX11Context::DX11Context(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(windowHandle, "Window handle is null!");

		RendererAPI::SetAPI(RendererAPI::API::DX11);
	}

	void DX11Context::Init()
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

	void DX11Context::SwapBuffers()
	{
		// glfwSwapBuffers(m_WindowHandle);
	}
}
