#include "ParticlesLayer.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Glad/include/glad/glad.h>

#include "Hazel/Models/Cube.h"
#include "Hazel/Renderer/CameraController.h"


ParticlesLayer::ParticlesLayer()
	: Layer("Particles Layer"), m_CameraController(16.f / 9.f, true)
{
}

void ParticlesLayer::OnAttach()
{
	Hazel::RenderCommand::Init();

	// ParticleSystem Init here
	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f;
	m_Particle.SizeEnd = 0.0f;
	m_Particle.SizeVariation = 0.3f;
	m_Particle.LifeTime = 1.0f;
	m_Particle.Velocity = { 0.0f, 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 3.0f, 3.0f };
	m_Particle.Position = { 0.0f, 0.0f, 0.0f };
}

void ParticlesLayer::OnDetach()
{
	// Shutdown here
}

void ParticlesLayer::OnEvent(Hazel::Event& event)
{
	m_CameraController.OnEvent(event);
}

void ParticlesLayer::OnUpdate(Hazel::Timestep timestep)
{
	// Update
	m_CameraController.OnUpdate(timestep);

	m_FPS = (unsigned int)(1.0f / timestep.GetSeconds());

	// Render
	Hazel::RenderCommand::SetClearColor(m_BackgroundColor);
	Hazel::RenderCommand::Clear();

	Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

	// Render here
	if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Hazel::Input::GetMousePosition();
		float z = 0;
		auto width = Hazel::Application::Get().GetWindow().GetWidth();
		auto height = Hazel::Application::Get().GetWindow().GetHeight();

		// auto bounds = m_CameraController.GetBounds();
		float boundsWidth = 16.0f; // bounds.GetWidth();
		float boundsHeight = 9.0f; // bounds.GetHeight();
		float boundsDepth = -16.0f;
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * boundsWidth - boundsWidth * 0.5f;
		y = boundsHeight * 0.5f - (y / height) * boundsHeight;
		z = boundsDepth * 0.5f - (y / height) * boundsDepth;
		m_Particle.Position = { x + pos.x, y + pos.y, z + pos.z };
		for (int i = 0; i < 5; i++)
		{
			m_ParticleSystem.Emit(m_Particle);
		}
	}

	m_ParticleSystem.OnUpdate(timestep);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());

	Hazel::Renderer::EndScene();
}

void ParticlesLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Birth Color", glm::value_ptr(m_Particle.ColorBegin));
	ImGui::ColorEdit4("Death Color", glm::value_ptr(m_Particle.ColorEnd));
	ImGui::DragFloat("Life Time", &m_Particle.LifeTime, 0.1f, 0.0f, 1000.0f);
	ImGui::Value("FPS", m_FPS);
	ImGui::End();
}
