#include "hzpch.h"

#include "Framebuffer.h"
#include "Renderer2D.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"


namespace Hazel {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLFramebuffer>(spec);
		default:
			HZ_CORE_ASSERT(false, "RendererAPI value unknown!"); return nullptr;
		}

	}
}
