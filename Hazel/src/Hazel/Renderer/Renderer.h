#pragma once

#include "RendererAPI.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "RenderPass.h"
#include "Material.h"
#include "Mesh.h"


namespace Hazel
{
	class Renderer
	{
	public:

		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void BeginScene(Camera& camera);
		static void EndScene();

		// Primitives
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture);
		static void DrawCube(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture);
		static void DrawCube(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture);

		static void Submit(Ref<Shader>& shader, const Ref<VertexArray>& vertexArray,
			const glm::mat4& viewProjectionMatrix, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void BeginRenderPass(const Ref<RenderPass>& renderPass);
		static void SubmitFullscreenQuad(Ref<MaterialInstance>& materialInstance);
		static void SubmitMesh(Ref<Mesh>& mesh, glm::mat4& transform, Ref<MaterialInstance>& material);
		static void SubmitQuad(Ref<MaterialInstance>& material, glm::mat4& transform);

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}
