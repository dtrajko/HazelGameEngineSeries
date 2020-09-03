#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


namespace Hazel {

	class VulkanDevice
	{
	public:
		VulkanDevice();
		~VulkanDevice();

		VkQueue GetQueue();

	private:
		VkDevice m_LogicalDevice = nullptr;

	};

	class VulkanPhysicalDevice
	{
	public:
		VulkanPhysicalDevice();
		~VulkanPhysicalDevice();

		static Scope<VulkanPhysicalDevice> Select();

	private:
		VkPhysicalDevice m_PhysicalDevice = nullptr;
		VkPhysicalDeviceProperties m_Properties;
		VkPhysicalDeviceFeatures m_Features;
		VkPhysicalDeviceMemoryProperties m_MemoryProperties;

		std::vector<VkQueueFamilyProperties> m_QueueFamilyProperties;
		std::vector<std::string> m_SupportedExtensions;
		std::vector<VkDeviceQueueCreateInfo> m_QueueCreateInfo;

	};

}
