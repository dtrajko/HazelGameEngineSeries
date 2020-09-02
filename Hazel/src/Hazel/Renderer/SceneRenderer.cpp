#include "hzpch.h"

#include "SceneRenderer.h"
#include "Renderer3D.h"
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
	}

	void SceneRenderer::BeginScene(Scene* scene)
	{
	}

	void SceneRenderer::SubmitEntity(Entity* entity)
	{
	}

	void SceneRenderer::EndScene()
	{
	}

	void SceneRenderer::CreateEnvironmentMap(const std::string& filepath)
	{
	}

	Ref<Texture2D> SceneRenderer::GetFinalColorBuffer()
	{
		return Ref<Texture2D>();
	}

	uint32_t SceneRenderer::GetFinalColorBufferRendererID()
	{
		return uint32_t();
	}

	void SceneRenderer::FlushDrawList()
	{
	}

	void SceneRenderer::GeometryPass()
	{
	}

	void SceneRenderer::CompositePass()
	{
	}

}
