#pragma once

#include "RendererAPI.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "RenderPass.h"
#include "Material.h"
#include "Mesh.h"
#include "RenderCommandQueue.h"


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

		template<typename FuncT>
		static void Submit(FuncT&& func)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();

				// NOTE: Instead of destroying we could try and enforce all items to be trivially destructible
				// however some items like uniforms which contain std::strings still exist for now
				// static_assert(std::is_trivially_destructible_v<FuncT>, "FuncT must be trivially destructible");
				pFunc->~FuncT();
			};
			auto storageBuffer = GetRenderCommandQueue().Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}

		//	static void Submit(Ref<Shader>& shader, const Ref<VertexArray>& vertexArray,
		//		const glm::mat4& viewProjectionMatrix, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void SubmitFullscreenQuad(const Ref<MaterialInstance>& materialInstance);
		static void SubmitMesh(const Ref<Mesh>& mesh, const glm::mat4& transform, const Ref<MaterialInstance>& material);
		static void SubmitQuad(const Ref<MaterialInstance>& material, const glm::mat4& transform);

		void Clear();
		void Clear(float r, float g, float b);
		void ClearMagenta();
		void SetClearColor(float r, float g, float b, float a);
		static void DrawIndexed(uint32_t count, bool depthTest);
		void WaitAndRender();
		static void BeginRenderPass(const Ref<RenderPass>& renderPass);
		static void EndRenderPass();

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;

	private:
		static RenderCommandQueue& GetRenderCommandQueue();
	};

}
