#pragma once


namespace Hazel
{

	class Quad
	{
	public:
		inline static float vertices[5 * 4] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};

		inline static uint32_t indices[2 * 3] = {
			 0, 1, 2,
			 2, 3, 0
		};
	};
}
