#include "hzpch.h"

#include "VertexArray.h"
#include "Renderer.h"
#include "Hazel/Platform/OpenGL/OpenGLVertexArray.h"


namespace Hazel
{

	VertexArray* VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
		default:
			HZ_CORE_ASSERT(false, "RendererAPI value unknown!");
			return nullptr;
		}
	}

}
