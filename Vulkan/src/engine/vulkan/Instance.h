#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>


class Debug;


class Instance
{
public:

	// Vulkan
	VkInstance hInstance;


public:

	Instance(bool enableValidationLayers);

	std::vector<const char*> getRequiredExtensions(bool enableValidationLayers);

	~Instance();


};
