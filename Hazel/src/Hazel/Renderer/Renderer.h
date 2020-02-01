#pragma once

#include "RendererAPI.h"
#include "RenderCommand.h"
#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Renderer/Shader.h"


namespace Hazel
{
	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;

	};

}
