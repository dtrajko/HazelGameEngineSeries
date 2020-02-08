#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>



class ValidationLayer
{
public:

	static const std::vector<const char*> validationLayers;

public:

	static bool checkValidationLayerSupport();

};
