#pragma once

#include "Entity.h"
#include "Hazel/Renderer/Camera.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Renderer/TextureCube.h"

#include <vector>


namespace Hazel {

	class Scene
	{
	public:
		Scene(const std::string& debugName = "Scene");
		~Scene();

		void Init();

		void OnUpdate(Timestep ts);

		void SetCamera(const Camera& camera);
		Camera& GetCamera() { return m_Camera; }

		void LoadEnvironmentMap(const std::string& filepath);
		void SetEnvironmentMaps(const Ref<TextureCube>& environmentRadianceMap, const Ref<TextureCube>& environmentIrradianceMap);
		void SetSkybox(const Ref<TextureCube>& skybox);

		void AddEntity(Entity* entity);
		Entity* CreateEntity();

	private:
		const std::string& m_DebugName;
		std::vector<Entity*> m_Entities;
		Camera m_Camera;

		Ref<TextureCube> m_EnvironmentRadianceMap;
		Ref<TextureCube> m_EnvironmentIrradianceMap;
		Ref<TextureCube> m_SkyboxTexture;
		Ref<MaterialInstance> m_SkyboxMaterial;

		friend class SceneRenderer;
	};

}
