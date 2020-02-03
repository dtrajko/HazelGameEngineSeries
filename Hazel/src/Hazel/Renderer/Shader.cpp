#include "hzpch.h"
#include "Shader.h"

#include "Hazel/Renderer/RendererAPI.h"
#include "Hazel/Platform/OpenGL/OpenGLShader.h"


namespace Hazel
{

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertexSrc, fragmentSrc);
		default:
			HZ_CORE_ASSERT(false, "RendererAPI value unknown!");
			return nullptr;
		}
	}

}
