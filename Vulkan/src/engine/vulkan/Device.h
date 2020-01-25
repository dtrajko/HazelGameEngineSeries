#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class PhysicalDevice;


class Device
{

public:

	VkDevice m_Device;

	// Vulkan logical device
	VkQueue graphicsQueue;

	// Presentation queue
	VkQueue presentQueue;

public:

	Device(PhysicalDevice* physicalDevice, VkSurfaceKHR surfaceKHR, bool enableValidationLayers);

	~Device();

};
