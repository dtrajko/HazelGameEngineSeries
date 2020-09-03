#include "hzpch.h"

#include "SceneRenderer.h"
#include "Renderer.h"
#include "RenderPass.h"
#include "Framebuffer.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>


namespace Hazel {

	struct SceneRendererData
	{
		const Scene* ActiveScene = nullptr;
		struct SceneInfo
		{
			Camera SceneCamera;
		
			// Resources
			Ref<MaterialInstance> SkyboxMaterial;
			Ref<TextureCube> EnvironmentRadianceMap;
			Ref<TextureCube> EnvironmentIrradianceMap;
		
		} SceneData;

		Ref<Texture2D> BRDFLUT;
		Ref<Shader> CompositeShader;
		
		Ref<RenderPass> GeoPass;
		Ref<RenderPass> CompositePass;
		
		struct DrawCommand
		{
			Ref<Mesh> Mesh;
			Ref<MaterialInstance> Material;
			glm::mat4 Transform;
		};
		std::vector<DrawCommand> DrawList;
		
		// Grid
		Ref<MaterialInstance> GridMaterial;
	};

	static SceneRendererData s_Data;

	void SceneRenderer::Init()
	{
		FramebufferSpecification geoFramebufferSpec;
		geoFramebufferSpec.Width = 1280;
		geoFramebufferSpec.Height = 720;
		geoFramebufferSpec.Format = FramebufferFormat::RGBA16F;
		geoFramebufferSpec.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };

		RenderPassSpecification geoRenderPassSpec;
		geoRenderPassSpec.TargetFramebuffer = Hazel::Framebuffer::Create(geoFramebufferSpec);
		s_Data.GeoPass = RenderPass::Create(geoRenderPassSpec);

		FramebufferSpecification compFramebufferSpec;
		compFramebufferSpec.Width = 1280;
		compFramebufferSpec.Height = 720;
		compFramebufferSpec.Format = FramebufferFormat::RGBA8;
		compFramebufferSpec.ClearColor = { 0.5f, 0.1f, 0.1f, 1.0f };

		RenderPassSpecification compRenderPassSpec;
		compRenderPassSpec.TargetFramebuffer = Framebuffer::Create(compFramebufferSpec);
		s_Data.CompositePass = RenderPass::Create(compRenderPassSpec);

		s_Data.CompositeShader = Shader::Create("assets/shaders/hdr.glsl");
		s_Data.BRDFLUT = Texture2D::Create("assets/textures/BRDF_LUT.tga");

		// Grid
		auto gridShader = Shader::Create("assets/shaders/Grid.glsl");
		s_Data.GridMaterial = MaterialInstance::Create(Material::Create(gridShader));
		float gridScale = 16.025f;
		float gridSize = 0.025f;
		s_Data.GridMaterial->Set("u_Scale", gridScale);
		s_Data.GridMaterial->Set("u_Res", gridSize);
	}

	void SceneRenderer::SetViewportSize(uint32_t width, uint32_t height)
	{
		s_Data.GeoPass->GetSpecification().TargetFramebuffer->Resize(width, height);
		s_Data.CompositePass->GetSpecification().TargetFramebuffer->Resize(width, height);
	}

	void SceneRenderer::BeginScene(Scene* scene)
	{
		HZ_CORE_ASSERT(!s_Data.ActiveScene, "");

		s_Data.ActiveScene = scene;

		s_Data.SceneData.SceneCamera = scene->m_Camera;
		s_Data.SceneData.SkyboxMaterial = scene->m_SkyboxMaterial;
		s_Data.SceneData.EnvironmentRadianceMap = scene->m_EnvironmentRadianceMap;
		s_Data.SceneData.EnvironmentIrradianceMap = scene->m_EnvironmentIrradianceMap;
	}

	void SceneRenderer::EndScene()
	{
		HZ_CORE_ASSERT(s_Data.ActiveScene, "");

		s_Data.ActiveScene = nullptr;

		FlushDrawList();
	}

	void SceneRenderer::SubmitEntity(Entity* entity)
	{
		// TODO: Culling, sorting, etc.

		auto mesh = entity->GetMesh();
		if (!mesh)
			return;

		s_Data.DrawList.push_back({ mesh, entity->GetMaterial(), entity->GetTransform() });
	}

	void SceneRenderer::CreateEnvironmentMap(const std::string& filepath)
	{
		const uint32_t cubemapSize = 1024;

		Ref<TextureCube> envUnfiltered = TextureCube::Create(TextureFormat::Float16, cubemapSize, cubemapSize);
		Ref<Shader> equirectangularConversionShader = Shader::Create("assets/shaders/EquirectangularToCubeMap.glsl");
		Ref<Texture2D> envEquirect = Texture2D::Create(filepath);
		HZ_CORE_ASSERT(envEquirect->GetFormat() == TextureFormat::Float16, "Texture is not HDR!");

		equirectangularConversionShader->Bind();
		envEquirect->Bind(0);
		//	Renderer::Submit([=]()
		//	{
		//			glBindImageTexture(0, envUnfiltered->GetRendererID(), 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16F);
		//			glDispatchCompute(cubemapSize / 32, cubemapSize / 32, 6);
		//			glGenerateTextureMipmap(envUnfiltered->GetRendererID());
		//	});
	}

	void SceneRenderer::GeometryPass()
	{
		Renderer::BeginRenderPass(s_Data.GeoPass);
		auto viewProjection = s_Data.SceneData.SceneCamera.GetProjectionMatrix() * s_Data.SceneData.SceneCamera.GetViewMatrix();

		// Skybox
		auto skyboxShader = s_Data.SceneData.SkyboxMaterial->GetShader();
		s_Data.SceneData.SkyboxMaterial->Set("u_InverseVP", glm::inverse(viewProjection));
		Renderer::SubmitFullscreenQuad(s_Data.SceneData.SkyboxMaterial);

		// Render entities
		for (auto& dc : s_Data.DrawList)
		{
			dc.Material->Set("u_ViewProjectionMatrix", viewProjection);
			dc.Material->Set("u_CameraPosition", s_Data.SceneData.SceneCamera.GetPosition());

			// Environment (TODO: don't do this per mesh)
			dc.Material->Set("u_EnvRadianceTex", s_Data.SceneData.EnvironmentRadianceMap);
			dc.Material->Set("u_EnvIrradianceTex", s_Data.SceneData.EnvironmentIrradianceMap);
			dc.Material->Set("u_BRDFLUTTexture", s_Data.BRDFLUT);

			Renderer::SubmitMesh(dc.Mesh, dc.Transform, dc.Material);
		}

		// Grid
		s_Data.GridMaterial->Set("u_ViewProjection", viewProjection);
		Renderer::SubmitQuad(s_Data.GridMaterial, glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));

		Renderer::EndRenderPass();
	}

	void SceneRenderer::CompositePass()
	{
		Renderer::BeginRenderPass(s_Data.CompositePass);
		s_Data.CompositeShader->Bind();
		s_Data.CompositeShader->SetFloat("u_Exposure", s_Data.SceneData.SceneCamera.GetExposure());
		s_Data.GeoPass->GetSpecification().TargetFramebuffer->BindTexture();
		Renderer::SubmitFullscreenQuad(nullptr);
		Renderer::EndRenderPass();

	}

	void SceneRenderer::FlushDrawList()
	{
		HZ_CORE_ASSERT(!s_Data.ActiveScene, "");
	}

	Ref<Texture2D> SceneRenderer::GetFinalColorBuffer()
	{
		return Ref<Texture2D>();
	}

	uint32_t SceneRenderer::GetFinalColorBufferRendererID()
	{
		return uint32_t();
	}

}
