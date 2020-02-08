#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class Format
{
public:

	static bool hasStencilComponent(VkFormat format);

};
