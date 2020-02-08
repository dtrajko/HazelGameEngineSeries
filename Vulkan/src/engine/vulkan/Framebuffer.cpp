#include "Framebuffer.h"

#include "SwapChain.h"
#include "Image.h"
#include "ImageView.h"
#include "ImageFactory.h"

#include <array>
#include <stdexcept>



std::vector<VkFramebuffer> Framebuffer::swapChainFramebuffers = std::vector<VkFramebuffer>();


void Framebuffer::createFramebuffers(VkDevice device, SwapChain* swapChain, ImageFactory* imageFactory, VkRenderPass renderPass)
{
	swapChainFramebuffers.resize(swapChain->swapChainImageViews.size());

	for (size_t i = 0; i < swapChain->swapChainImageViews.size(); i++)
	{
		std::array<VkImageView, 3> attachments = {
			imageFactory->imageColor->m_ImageView->m_ImageView,
			imageFactory->imageDepth->m_ImageView->m_ImageView,
			swapChain->swapChainImageViews[i],
		};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapChain->swapChainExtent.width;
		framebufferInfo.height = swapChain->swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create framebuffer!");
		}
	}
}
