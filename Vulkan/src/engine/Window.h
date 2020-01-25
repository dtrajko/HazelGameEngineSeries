#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class Window
{

public:

	// GLFW
	GLFWwindow* m_Window;

	const int WIDTH = 1280;
	const int HEIGHT = 720;

	// Framebuffers
	bool framebufferResized = false; // used to recreate the swap chain


public:

	Window();

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

	void OnWindowResize(int width, int height);
	void OnWindowClose();
	void OnKeyPressed(int key, int scancode, int action, int mods, int repeatCount);
	void OnKeyReleased(int key, int scancode, int action, int mods);
	void OnMouseButtonPressed(int button, int action, int modes);
	void OnMouseButtonReleased(int button, int action, int modes);
	void OnMouseScrolled(float xOffset, float yOffset);
	void OnMouseMoved(float xPos, float yPos);

	~Window();

};
