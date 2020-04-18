#include "hzpch.h"

#include "VertexArray.h"
#include "Renderer2D.h"
#include "Hazel/Platform/OpenGL/OpenGLVertexArray.h"


namespace Hazel
{

	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		default:
			HZ_CORE_ASSERT(false, "RendererAPI value unknown!");
			return nullptr;
		}
	}

}
