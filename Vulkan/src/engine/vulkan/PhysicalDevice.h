#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>
#include <vector>


// Swapchain capability
const std::vector<const char*> deviceExtensions =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};


class PhysicalDevice
{

public:

	// Vulkan physical devices
	VkPhysicalDevice m_Device = VK_NULL_HANDLE;

public:

	PhysicalDevice(VkInstance instance, VkSurfaceKHR surfaceKHR, VkSampleCountFlagBits& msaaSamples);

	~PhysicalDevice();

	bool isDeviceSuitable(VkPhysicalDevice hPhysicalDevice, VkSurfaceKHR surfaceKHR);

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surfaceKHR);

	bool checkDeviceExtensionSupport(VkPhysicalDevice hPhysicalDevice);

	int rateDeviceSuitability();

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice hPhysicalDevice, VkSurfaceKHR& surface);

	VkSampleCountFlagBits getMaxUsableSampleCount();

};
