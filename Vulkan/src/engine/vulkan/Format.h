#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class Format
{
public:

	bool hasStencilComponent(VkFormat format);

};
