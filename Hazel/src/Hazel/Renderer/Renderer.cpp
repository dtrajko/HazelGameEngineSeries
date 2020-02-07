#include "hzpch.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "Hazel/Platform/OpenGL/OpenGLShader.h"


namespace Hazel
{

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;


	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::BeginScene(glm::mat4 viewProjectionMatrix)
	{
		m_SceneData->ViewProjectionMatrix = viewProjectionMatrix;
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(Ref<Shader>& shader, const Ref<VertexArray>& vertexArray,
		const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}
