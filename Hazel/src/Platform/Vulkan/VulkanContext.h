#pragma once

#include "Hazel/Renderer/GraphicsContext.h"


struct GLFWwindow;


namespace Hazel
{
	class VulkanContext : public GraphicsContext
	{

	public:

		VulkanContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;

	};

}
