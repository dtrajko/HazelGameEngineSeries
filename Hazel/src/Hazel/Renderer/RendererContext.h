#pragma once


namespace Hazel
{
	class RendererContext
	{

	public:

		virtual void Create() = 0;
		virtual void SwapBuffers() = 0;

		virtual void OnResize(uint32_t width, uint32_t height) = 0;

	};

}
