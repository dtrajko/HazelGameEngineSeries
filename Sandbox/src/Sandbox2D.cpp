#include "Sandbox2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


static const uint32_t s_MapWidth = 24;
static const char* s_MapTiles =
	"WWWWWWWWWWWWWWWWWWWWWWWW"
	"WWWWDDDDDDDDDDDWWWWWWWWW"
	"WWDDDDWDWDDDDDDDDDDWWWWW"
	"WWDDDWWWWWDDDDDDDDDDWWWW"
	"WWDDDWWWWWWDDDDDDDDDDDWW"
	"WWDDDDWWWWWDDDDDDDDDDDWW"
	"WWDDDDDWWWDDDDDDDDDDDDWW"
	"WWDDDDDDDDDRDDDDDDDDDDWW"
	"WWDDDDDDDDDDDDDDDDDDDWWW"
	"WWWDDDDDDDDDDDDDDDDDWWWW"
	"WWWWDDDDDDDDDDDDDDWWWWWW"
	"WWWWWWDDDDDDDDDWWWWWWWWW"
	"WWWWWWWWWWWWWWWWWWWWWWWW";

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(16.f / 9.f, true)
{
	Hazel::RendererAPI::SetMode(Hazel::RendererAPI::Mode::Renderer2D);
}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();

	m_CameraController.SetZoomLevel(6.5f);

	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
	m_QuadTexture = Hazel::Texture2D::Create("assets/textures/statue_512x512.jpg");
	m_SpriteSheet = Hazel::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");

	m_TextureStairs = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 11 }, { 128, 128 }, { 1, 1 });
	m_TextureTree   = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2,  1 }, { 128, 128 }, { 1, 2 });
	m_TextureBarrel = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 9,  2 }, { 128, 128 }, { 1, 1 });

	m_MapWidth = s_MapWidth;
	m_MapHeight = (uint32_t)strlen(s_MapTiles) / s_MapWidth;

	s_TextureMap['D'] = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, {  6, 11 }, { 128, 128 }, { 1, 1 });
	s_TextureMap['W'] = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128, 128 }, { 1, 1 });

	// Init here
	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 4.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
}

void Sandbox2D::OnDetach()
{
	HZ_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Hazel::Timestep timestep)
{
	HZ_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(timestep);

	// Render
	Hazel::Renderer2D::ResetStats();

	{
		HZ_PROFILE_SCOPE("Renderer Prep");
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();
	}

#if 0
	{
		static float rotation = 0.0f;
		rotation += timestep = 10.0f;

		HZ_PROFILE_SCOPE("Renderer Draw");
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Hazel::Renderer2D::DrawRotatedQuad({ 0.6f, -0.4f }, { 0.8f, 0.8f }, glm::radians(rotation), { 0.8f, 0.2f, 0.3f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.8f,  0.4f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.0f, -0.3f }, { 0.5f, 0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, 10.0f);
		Hazel::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f,  0.0f }, { 3.0f, 3.0f }, glm::radians(45.0f), m_CheckerboardTexture, 10.0f);
		Hazel::Renderer2D::EndScene();
		
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y <= 5.0f; y += 0.25f)
		{
			for (float x = -5.0f; x <= 5.0f; x += 0.25f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 1.0f };
				Hazel::Renderer2D::DrawQuad({ x, y }, { 0.23f, 0.23f }, color);
			}
		}
		
		Hazel::Renderer2D::EndScene();
	}
#endif

	if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Hazel::Input::GetMousePosition();
		auto width = Hazel::Application::Get().GetWindow().GetWidth();
		auto height = Hazel::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 10; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	// ParticleSystem Update & Render
	m_ParticleSystem.OnUpdate(timestep); // 0.01f
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

	for (uint32_t y = 0; y < m_MapHeight; y++)
	{
		for (uint32_t x = 0; x < m_MapWidth; x++)
		{
			char tileType = s_MapTiles[x + y * m_MapWidth];
			Hazel::Ref<Hazel::SubTexture2D> texture;
			if (s_TextureMap.find(tileType) != s_TextureMap.end())
				texture = s_TextureMap[tileType];
			else
				texture = m_TextureBarrel;

			Hazel::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f + 0.5f, m_MapHeight - y - m_MapHeight / 2.0f - 0.5f, 0.0f }, { 1.0f, 1.0f }, texture);
		}
	}

	// Load a Sprite sheet
	Hazel::Renderer2D::DrawQuad({ -1.5f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_TextureStairs);
	Hazel::Renderer2D::DrawQuad({  0.0f, 0.0f, 0.0f }, { 1.0f, 2.0f }, m_TextureTree);
	Hazel::Renderer2D::DrawQuad({  1.5f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_TextureBarrel);

	Hazel::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	HZ_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = Hazel::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}
