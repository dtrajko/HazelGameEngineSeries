#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>


class ShaderModule
{
public:

	VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code);
};
