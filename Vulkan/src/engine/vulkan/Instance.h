#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>


class ValidationLayer;
class Debug;


class Instance
{
public:

	// Vulkan
	VkInstance hInstance;


public:

	Instance(bool enableValidationLayers, const std::vector<const char*> validationLayers, ValidationLayer validationLayer);

	std::vector<const char*> getRequiredExtensions(bool enableValidationLayers);

	~Instance();


};
