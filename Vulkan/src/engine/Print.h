#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan/SwapChain.h"

#include <iostream>


class Print
{

public:

	static void printDeviceProperties(
		VkPhysicalDeviceProperties deviceProperties,
		VkPhysicalDeviceFeatures deviceFeatures,
		int score)
	{
		std::cout << std::endl;
		std::cout << "Device Properties" << std::endl;
		std::cout << "\t" << "deviceID: " << deviceProperties.deviceID << std::endl;
		std::cout << "\t" << "apiVersion: " << deviceProperties.apiVersion << std::endl;
		std::cout << "\t" << "deviceName: " << deviceProperties.deviceName << std::endl;
		std::cout << "\t" << "deviceType: " << deviceProperties.deviceType << std::endl;
		std::cout << "\t" << "driverVersion: " << deviceProperties.driverVersion << std::endl;
		std::cout << "\t" << "limits.bufferImageGranularity: " << deviceProperties.limits.bufferImageGranularity << std::endl;
		std::cout << "\t" << "limits.discreteQueuePriorities: " << deviceProperties.limits.discreteQueuePriorities << std::endl;
		std::cout << "\t" << "limits.framebufferColorSampleCounts: " << deviceProperties.limits.framebufferColorSampleCounts << std::endl;
		std::cout << "\t" << "..." << std::endl;
		std::cout << "\t" << "vendorID: " << deviceProperties.vendorID << std::endl;
		std::cout << "Device Features" << std::endl;
		std::cout << "\t" << "geometryShader: " << deviceFeatures.geometryShader << std::endl;
		std::cout << "\t" << "sparseBinding: " << deviceFeatures.sparseBinding << std::endl;
		std::cout << "\t" << "..." << std::endl;
		std::cout << "Physical device score: " << score << std::endl;
	}

	static void printSwapChainSupport(bool swapChainAdequate, SwapChainSupportDetails swapChainSupport)
	{
		std::cout << std::endl;
		std::cout << "SwapChain Support: " << std::endl;
		std::cout << "\t" << "swapChainAdequate: " << (swapChainAdequate ? "YES" : "NO") << std::endl;
		std::cout << "\t" << "Number of Formats: " << swapChainSupport.formats.size() << std::endl;
		for (VkSurfaceFormatKHR format : swapChainSupport.formats)
		{
			std::cout << "\t\t" << "Format: " << format.format << std::endl;
		}
		std::cout << "\t" << "Number of Presentation Modes: " << swapChainSupport.presentModes.size() << std::endl;
		for (VkPresentModeKHR presentMode : swapChainSupport.presentModes)
		{
			std::cout << "\t\t" << "Format: " << presentMode << std::endl;
		}
	}
};
