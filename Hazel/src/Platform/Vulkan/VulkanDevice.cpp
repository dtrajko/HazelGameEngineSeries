#include "hzpch.h"
#include "VulkanDevice.h"


namespace Hazel {

	VulkanDevice::VulkanDevice()
	{

	}

	VulkanDevice::~VulkanDevice()
	{

	}

	VkQueue VulkanDevice::GetQueue()
	{
		return VkQueue();
	}

	VulkanPhysicalDevice::VulkanPhysicalDevice()
	{

	}

	VulkanPhysicalDevice::~VulkanPhysicalDevice()
	{

	}

	Scope<VulkanPhysicalDevice> VulkanPhysicalDevice::Select()
	{
		return Scope<VulkanPhysicalDevice>();
	}

}
