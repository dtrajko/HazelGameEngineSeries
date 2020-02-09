#include "hzpch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "RenderCommand.h"
#include "Hazel/Models/Cube.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Hazel
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	struct RendererStorage
	{
		Ref<VertexArray> CubeVertexArray;
		Ref<Shader> FlatColorShader;
		Ref<Shader> TextureShader;
	};

	static RendererStorage* s_Data;

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();

		s_Data = new RendererStorage();
		s_Data->CubeVertexArray = VertexArray::Create();

		Ref<VertexBuffer> cubeVB;
		cubeVB = VertexBuffer::Create(Cube::vertices, sizeof(Cube::vertices));
		cubeVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			});

		s_Data->CubeVertexArray->AddVertexBuffer(cubeVB);

		Ref<IndexBuffer> cubeIB;
		cubeIB = IndexBuffer::Create(Cube::indices, sizeof(Cube::indices) / sizeof(uint32_t));
		s_Data->CubeVertexArray->SetIndexBuffer(cubeIB);

		s_Data->FlatColorShader = Shader::Create("assets/shaders/Renderer2D_FlatColor.glsl");
		s_Data->TextureShader = Shader::Create("assets/shaders/Renderer2D_Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(Camera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();

		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer::Shutdown()
	{
		delete s_Data;
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), size);
		s_Data->FlatColorShader->SetMat4("u_Transform", transform);

		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
	}

	void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture)
	{
		s_Data->TextureShader->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), size);
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		texture->Bind();

		s_Data->CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->CubeVertexArray);
	}

	void Renderer::Submit(Ref<Shader>& shader, const Ref<VertexArray>& vertexArray,
		const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}
