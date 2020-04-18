#pragma once


namespace Hazel
{

	class Quad
	{
	public:
		inline static float vertices[4 * 9] =
		{
			//  X      Y     Z        R     G     B     A        U     V
			-0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f,    1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f,    1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f,
		};

		inline static uint32_t indices[2 * 3] = {
			 0, 1, 2,
			 2, 3, 0
		};
	};
}
