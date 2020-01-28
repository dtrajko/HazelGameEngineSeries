#include "PhysicalDevice.h"

#include "../Print.h"

#include <stdexcept>
#include <string>
#include <set>


PhysicalDevice::PhysicalDevice(VkInstance instance, VkSurfaceKHR surfaceKHR, VkSampleCountFlagBits& msaaSamples)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	int physicalDeviceScore = 0;

	if (deviceCount == 0)
	{
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> phyDevices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, phyDevices.data());

	for (const auto& phyDevice : phyDevices)
	{
		if (isDeviceSuitable(phyDevice, surfaceKHR))
		{
			m_Device = phyDevice;
			msaaSamples = getMaxUsableSampleCount();
			break;
		}
	}

	if (m_Device == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Failed to find a suitable GPU!");
	}
}

PhysicalDevice::~PhysicalDevice()
{
}

bool PhysicalDevice::isDeviceSuitable(VkPhysicalDevice hPhysicalDevice, VkSurfaceKHR surfaceKHR)
{
	QueueFamilyIndices indices = findQueueFamilies(hPhysicalDevice, surfaceKHR);

	bool extensionsSupported = checkDeviceExtensionSupport(hPhysicalDevice);

	bool swapChainAdequate = false;
	SwapChainSupportDetails swapChainSupport;

	if (extensionsSupported)
	{
		swapChainSupport = querySwapChainSupport(hPhysicalDevice, surfaceKHR);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(hPhysicalDevice, &supportedFeatures);

	// Print::printSwapChainSupport(swapChainAdequate, swapChainSupport);

	return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

SwapChainSupportDetails PhysicalDevice::querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surfaceKHR)
{
	SwapChainSupportDetails details;

	// Step 1
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surfaceKHR, &details.capabilities);

	// Step 2
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surfaceKHR, &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surfaceKHR, &formatCount, details.formats.data());
	}

	// Step 3
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surfaceKHR, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surfaceKHR, &presentModeCount, details.presentModes.data());
	}

	return details;
}

bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice hPhysicalDevice)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(hPhysicalDevice, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(hPhysicalDevice, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	// Print::deviceExtensionSupport(availableExtensions);

	return requiredExtensions.empty();
}

int PhysicalDevice::rateDeviceSuitability()
{
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(m_Device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(m_Device, &deviceFeatures);

	int score = 0;

	// Discrete GPUs have a significant performance advantage
	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		score += 1000;
	}

	// Maximum possible size of textures affects graphics quality
	score += deviceProperties.limits.maxImageDimension2D;

	// Application can't function without geometry shaders
	if (!deviceFeatures.geometryShader)
	{
		score = 0;
	}

	// Print::printDeviceProperties(deviceProperties, deviceFeatures, score);

	return score;
}

uint32_t PhysicalDevice::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(m_Device, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type!");
}

QueueFamilyIndices PhysicalDevice::findQueueFamilies(VkPhysicalDevice hPhysicalDevice, VkSurfaceKHR& surface)
{
	// Logic to find graphics queue family
	QueueFamilyIndices indices = {};
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(hPhysicalDevice, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(hPhysicalDevice, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(hPhysicalDevice, i, surface, &presentSupport);

		if (presentSupport)
		{
			indices.presentFamily = i;
		}

		if (indices.isComplete())
		{
			break;
		}

		i++;
	}

	return indices;
}

VkSampleCountFlagBits PhysicalDevice::getMaxUsableSampleCount()
{
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(m_Device, &physicalDeviceProperties);

	VkSampleCountFlags counts =
		physicalDeviceProperties.limits.framebufferColorSampleCounts &
		physicalDeviceProperties.limits.framebufferDepthSampleCounts;

	if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
	if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
	if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
	if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
	if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
	if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

	return VK_SAMPLE_COUNT_1_BIT;
}
