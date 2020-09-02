#pragma once

#include "Texture.h"


namespace Hazel {

	class TextureCube
	{
	public:
		static Ref<TextureCube> Create(TextureFormat format, const uint32_t width, const uint32_t height);
		uint32_t GetRendererID() { return m_RendererID; };

	private:
		uint32_t m_RendererID;
	};

}
