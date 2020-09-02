#pragma once

#include "Hazel/Scene/Scene.h"
#include "Hazel/Renderer/Texture.h"


namespace Hazel {

	class SceneRenderer
	{
	public:
		static void Init();

		static void SetViewportSize(uint32_t width, uint32_t height);

		static void BeginScene(Scene* scene);
		static void EndScene();

		static void SubmitEntity(Entity* entity);

		static void CreateEnvironmentMap(const std::string& filepath);

		static Ref<Texture2D> GetFinalColorBuffer();

		// TODO: Temp
		static uint32_t GetFinalColorBufferRendererID();

	private:
		static void FlushDrawList();
		static void GeometryPass();
		static void CompositePass();

	};

}
