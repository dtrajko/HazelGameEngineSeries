#include "hzpch.h"
#include "Renderer2D.h"
#include "Renderer.h"
#include "Hazel/Models/Quad.h"
#include "Hazel/Models/Cube.h"
#include "RenderCommandQueue.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Hazel
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	struct RendererData
	{
		Ref<RenderPass> m_ActiveRenderPass;
		RenderCommandQueue m_CommandQueue;
		Scope<ShaderLibrary> m_ShaderLibrary;
		Ref<VertexArray> m_FullscreenQuadVertexArray;
	};

	static RendererData s_Data;

	void Renderer::Init()
	{
		s_Data.m_ShaderLibrary = std::make_unique<ShaderLibrary>();
		Renderer::Submit([]() { RendererAPI::Init(); });
	}

	void Renderer::Clear()
	{
		Renderer::Submit([]() {
			RendererAPI::Clear(0.0f, 0.0f, 0.0f, 1.0f);
		});
	}

	void Renderer::Clear(float r, float g, float b)
	{
	
	}

	void Renderer::ClearMagenta()
	{
		Clear(1, 0, 1);
	}

	void Renderer::SetClearColor(float r, float g, float b, float a)
	{
	}

	void Renderer::DrawIndexed(uint32_t count, bool depthTest)
	{
		Renderer::Submit([=]() {
			RendererAPI::DrawIndexed(count, depthTest);
		});
	}

	void Renderer::WaitAndRender()
	{
		s_Data.m_CommandQueue.Execute();
	}

	void Renderer::BeginRenderPass(const Ref<RenderPass>& renderPass)
	{
		HZ_CORE_ASSERT(renderPass, "Render pass cannot be null!");

		// TODO: Convert all of this into a render command buffer
		s_Data.m_ActiveRenderPass = renderPass;

		renderPass->GetSpecification().TargetFramebuffer->Bind();
		const glm::vec4& clearColor = renderPass->GetSpecification().TargetFramebuffer->GetSpecification().ClearColor;
		Renderer::Submit([=]() {
			RendererAPI::Clear(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		});
	}

	void Renderer::EndRenderPass()
	{
		HZ_CORE_ASSERT(s_Data.m_ActiveRenderPass, "No active render pass! Have you called Renderer::EndRenderPass twice?");
		s_Data.m_ActiveRenderPass->GetSpecification().TargetFramebuffer->Unbind();
		s_Data.m_ActiveRenderPass = nullptr;
	}

	RenderCommandQueue& Renderer::GetRenderCommandQueue()
	{
		return s_Data.m_CommandQueue;
	}

	void Renderer::SubmitQuad(const Ref<MaterialInstance>& material, const glm::mat4& transform)
	{
		bool depthTest = true;
		if (material)
		{
			material->Bind();
			depthTest = material->GetFlag(MaterialFlag::DepthTest);

			auto shader = material->GetShader();
			shader->SetMat4("u_Transform", transform);
		}

		s_Data.m_FullscreenQuadVertexArray->Bind();
		Renderer::DrawIndexed(6, depthTest);
	}

	void Renderer::SubmitFullscreenQuad(const Ref<MaterialInstance>& material)
	{
		bool depthTest = true;
		if (material)
		{

		}
	}

	void Renderer::SubmitMesh(const Ref<Mesh>& mesh, const glm::mat4& transform, const Ref<MaterialInstance>& material)
	{
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(Camera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();

		//	s_Data.TextureShader->Bind();
		//	s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer::Shutdown()
	{
		// delete s_Data;
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
	{
		//	s_Data.TextureShader->Bind();
		//	s_Data.TextureShader->SetFloat4("u_Color", color);
		//	
		//	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
		//		glm::scale(glm::mat4(1.0f), size);
		//	s_Data.TextureShader->SetMat4("u_Transform", transform);
		//	s_Data.WhiteTexture->Bind();
		//	
		//	s_Data.CubeVertexArray->Bind();
		//	RenderCommand::DrawIndexed(s_Data.CubeVertexArray);
	}

	void Renderer::DrawCube(const glm::mat4& transform, const glm::vec4& color)
	{
		//	s_Data.TextureShader->Bind();
		//	s_Data.TextureShader->SetFloat4("u_Color", color);
		//	s_Data.TextureShader->SetMat4("u_Transform", transform);
		//	s_Data.WhiteTexture->Bind();
		//	s_Data.CubeVertexArray->Bind();
		//	RenderCommand::DrawIndexed(s_Data.CubeVertexArray);
	}

	void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture)
	{
		//	s_Data.TextureShader->Bind();
		//	s_Data.TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		//	
		//	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
		//		glm::scale(glm::mat4(1.0f), size);
		//	s_Data.TextureShader->SetMat4("u_Transform", transform);
		//	s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		//	texture->Bind();
		//	s_Data.CubeVertexArray->Bind();
		//	RenderCommand::DrawIndexed(s_Data.CubeVertexArray);
	}

	void Renderer::DrawCube(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture)
	{
		//	s_Data.TextureShader->Bind();
		//	s_Data.TextureShader->SetFloat4("u_Color", color);
		//	s_Data.TextureShader->SetMat4("u_Transform", transform);
		//	s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		//	texture->Bind();
		//	s_Data.CubeVertexArray->Bind();
		//	RenderCommand::DrawIndexed(s_Data.CubeVertexArray);
	}

	void Renderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		//	s_Data.TextureShader->Bind();
		//	s_Data.TextureShader->SetFloat4("u_Color", color);
		//	s_Data.TextureShader->SetMat4("u_Transform", transform);
		//	s_Data.WhiteTexture->Bind();
		//	s_Data.QuadVertexArray->Bind();
		//	RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
	}

	void Renderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture)
	{
		//	s_Data.TextureShader->Bind();
		//	s_Data.TextureShader->SetFloat4("u_Color", color);
		//	s_Data.TextureShader->SetMat4("u_Transform", transform);
		//	s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		//	texture->Bind();
		//	s_Data.QuadVertexArray->Bind();
		//	RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
	}

	//	void Renderer::Submit(Ref<Shader>& shader, const Ref<VertexArray>& vertexArray,
	//		const glm::mat4& viewProjectionMatrix, const glm::mat4& transform)
	//	{
	//		shader->Bind();
	//		shader->SetMat4("u_ViewProjection", viewProjectionMatrix);
	//		shader->SetMat4("u_Transform", transform);
	//		shader->SetFloat("u_TilingFactor", 1.0f);
	//	
	//		vertexArray->Bind();
	//		RenderCommand::DrawIndexed(vertexArray);
	//	}

}
