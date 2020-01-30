#pragma once


namespace Hazel
{

	enum class RendererAPI
	{
		None = 0,
		OpenGL = 1,
		Vulkan = 2,
		DX11 = 3,
		DX12 = 4,
		Metal = 5,
	};

	class Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return s_RendererAPI; }

	private:
		static RendererAPI s_RendererAPI;

	};

}
