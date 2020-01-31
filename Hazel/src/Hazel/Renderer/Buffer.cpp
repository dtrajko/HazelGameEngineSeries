#include "hzpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Hazel/Platform/OpenGL/OpenGLBuffer.h"


namespace Hazel
{

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		default:
			HZ_CORE_ASSERT(false, "RendererAPI value unknown!");
			return nullptr;
		}
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLIndexBuffer(indices, size);
		default:
			HZ_CORE_ASSERT(false, "RendererAPI value unknown!");
			return nullptr;
		}
	}
}
