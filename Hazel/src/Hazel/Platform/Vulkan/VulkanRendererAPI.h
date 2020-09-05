#pragma once

#include "Hazel/Renderer/RendererAPI.h"


namespace Hazel
{

	class VulkanRendererAPI : public RendererAPI
	{
	public:
		VulkanRendererAPI() {};

		static void Init();
		static void Clear();
		static void Clear(float r, float g, float b) {};
		static void Clear(float r, float g, float b, float a) {};

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const glm::vec4& color) override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;

	};

}
