#include "Window.h"

#include "Input.h"

#include <iostream>


Window::Window()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	m_Window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	glfwSetWindowUserPointer(m_Window, this);
	glfwSetFramebufferSizeCallback(m_Window, framebufferResizeCallback);

	// Set GLFW callbacks
	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

			app->OnWindowResize(width, height);
		});

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

			app->OnWindowClose();
		});

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

			Input::Get()->m_Keys[key] = action != GLFW_RELEASE;

			switch (action)
			{
				case GLFW_PRESS:
				{
					app->OnKeyPressed(key, scancode, action, mods, 0);
					break;
				}
				case GLFW_RELEASE:
				{
					app->OnKeyReleased(key, scancode, action, mods);
					break;
				}
				case GLFW_REPEAT:
				{
					app->OnKeyPressed(key, scancode, action, mods, 1);
					break;
				}
			}
		});

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int modes)
		{
			auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

			Input::Get()->m_MouseButtons[button] = action != GLFW_RELEASE;

			switch (action)
			{
				case GLFW_PRESS:
				{
					app->OnMouseButtonPressed(button, action, modes);
					break;
				}
				case GLFW_RELEASE:
				{
					app->OnMouseButtonReleased(button, action, modes);
					break;
				}
			}
		});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

			app->OnMouseScrolled((float)xOffset, (float)yOffset);
		});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

			Input::Get()->SetMousePosition((float)xPos, (float)yPos);

			app->OnMouseMoved((float)xPos, (float)yPos);
			// TODO
		});

}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}

void Window::OnKeyPressed(int key, int scancode, int action, int mods, int repeatCount)
{
	std::cout << "Window::OnKeyPressed key: " << key << ", scancode: " << scancode << ", mods: " << mods << ", repeatCount: " << repeatCount << std::endl;
}

void Window::OnKeyReleased(int key, int scancode, int action, int mods)
{
	std::cout << "Window::OnKeyReleased key: " << key << ", scancode: " << scancode << ", mods: " << mods << std::endl;
}

void Window::OnMouseButtonPressed(int button, int action, int modes)
{
	std::cout << "Window::OnMouseButtonPressed button: " << button << ", action: " << action << ", modes: " << modes << std::endl;
}

void Window::OnMouseButtonReleased(int button, int action, int modes)
{
	std::cout << "Window::OnMouseButtonReleased button: " << button << ", action: " << action << ", modes: " << modes << std::endl;
}

void Window::OnMouseScrolled(float xOffset, float yOffset)
{
	std::cout << "Window::OnMouseScrolled xOffset: " << xOffset << ", yOffset: " << yOffset << std::endl;
}

void Window::OnMouseMoved(float xPos, float yPos)
{
	std::cout << "Window::OnMouseMoved xPos: " << xPos << ", yPos: " << yPos << std::endl;
}

void Window::OnWindowResize(int width, int height)
{
	std::cout << "Window::OnWindowResize width: " << width << ", height: " << height << std::endl;
}

void Window::OnWindowClose()
{
	std::cout << "Window::OnWindowClose" << std::endl;
}

Window::~Window()
{
}
