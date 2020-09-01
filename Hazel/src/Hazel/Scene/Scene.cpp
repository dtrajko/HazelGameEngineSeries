#include "hzpch.h"
#include "Scene.h"

#include "Hazel/Renderer/SceneRenderer.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Material.h"


namespace Hazel {

	Scene::Scene(const std::string& debugName)
		: m_DebugName(debugName), m_Camera(-1.0f, 1.0f, -1.0f, 1.0f)
	{
		Init();
	}

	Scene::~Scene()
	{
		for (Entity* entity : m_Entities)
			delete entity;
	}

	void Scene::Init()
	{
		Ref<Shader> skyboxShader = Shader::Create("assets/shaders/quad.glsl");
		m_SkyboxMaterial = MaterialInstance::Create(Material::Create(skyboxShader));
		m_SkyboxMaterial->SetFlag(MaterialFlag::DepthTest, false);

	}

	void Scene::OnUpdate(Timestep ts)
	{
		m_Camera.Update(ts);

		// Update all entities
		for (auto entity : m_Entities)
		{
			auto mesh = entity->GetMesh();
			if (mesh)
				mesh->OnUpdate(ts);
		}

		SceneRenderer::BeginScene(this);

		// Render entities
		for (auto entity : m_Entities)
		{
			// TODO: Should we render (logically)
			SceneRenderer::SubmitEntity(entity);
		}

		SceneRenderer::EndScene();
	}

	void Scene::SetCamera(const Camera& camera)
	{
		m_Camera = camera;
	}

	void Scene::LoadEnvironmentMap(const std::string& filepath)
	{
		SceneRenderer::CreateEnvironmentMap(filepath);
	}

	void Scene::SetEnvironmentMaps(const Ref<TextureCube>& environmentRadianceMap, const Ref<TextureCube>& environmentIrradianceMap)
	{
		m_EnvironmentRadianceMap = environmentRadianceMap;
		m_EnvironmentIrradianceMap = environmentIrradianceMap;
	}

	void Scene::SetSkybox(const Ref<TextureCube>& skybox)
	{
		m_SkyboxTexture = skybox;
		m_SkyboxMaterial->SetFlag("u_Texture", skybox);
	}

	void Scene::AddEntity(Entity* entity)
	{
		m_Entities.push_back(entity);
	}

	Entity* Scene::CreateEntity()
	{
		return nullptr;
	}

}
