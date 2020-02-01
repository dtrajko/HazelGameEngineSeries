#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"


namespace Hazel
{

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1,
			Vulkan = 2,
			DX11 = 3,
			DX12 = 4,
			Metal = 5,
		};

	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; };
		inline static void SetAPI(API api) { s_API = api; };

	private:
		static API s_API;


	};

}
