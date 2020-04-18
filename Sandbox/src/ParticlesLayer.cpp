#include "ParticlesLayer.h"

#include "Hazel/Models/Cube.h"
#include "Hazel/Renderer/CameraController.h"
#include "Hazel/Renderer/Renderer3D.h"
#include "Hazel/Core/Timer.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Glad/include/glad/glad.h>


#define PROFILE_SCOPE(name) Hazel::Timer timer##__LINE__(name, [&](ParticlesLayer::ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

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
	m_Particle.LifeTime = 4.0f;
	m_Particle.Position = { 0.0f, 0.0f, 0.0f };
	m_Particle.Velocity = { 0.0f, 0.0f, 0.0f };
	m_Particle.VelocityVariation = m_EnabledTexture ?
		glm::vec3(m_Velocity) * glm::vec3(0.0f, 0.0f, 0.0f) :
		glm::vec3(m_Velocity);
	m_Particle.Rotation = { 0.0f, 0.0f, 0.0f };
	m_Particle.RotationVelocity = glm::vec3(m_RotationVelocity);

	m_ParticleSystem.SetTexture(Hazel::Texture2D::Create("assets/textures/brick.png"));
	m_ParticleSystem.SetEnabled3D(m_Enabled3D);

	m_CameraController.GetCamera().SetFOV(glm::radians(45.0f));
	m_CameraController.SetZoomLevel(2.0f);
	m_CameraController.SetTranslationSpeed(2.0f);
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
	PROFILE_SCOPE("ParticlesLayer::OnUpdate");

	if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_MIDDLE))
	{
		bool cursorEnabled = false;
		Hazel::Application::Get().GetWindow().SetInputMode(cursorEnabled);
	}

	if (Hazel::Input::IsKeyPressed(HZ_KEY_ESCAPE))
	{
		bool cursorEnabled = true;
		Hazel::Application::Get().GetWindow().SetInputMode(cursorEnabled);
	}

	// Update
	m_CameraController.OnUpdate(timestep);

	// ParticleSystem Edit here
	m_Particle.VelocityVariation = glm::vec3(m_Velocity);
	m_Particle.RotationVelocity = glm::vec3(m_RotationVelocity);
	m_ParticleSystem.SetEnabled3D(m_Enabled3D);
	m_ParticleSystem.SetEnabledBillboarding(m_EnabledBillboarding);
	m_ParticleSystem.SetEnabledTexture(m_EnabledTexture);

	m_FPS = (unsigned int)(1.0f / timestep.GetSeconds());

	// Render
	{
		PROFILE_SCOPE("RenderCommand::SetClearColor");
		Hazel::RenderCommand::SetClearColor(m_BackgroundColor);
		Hazel::RenderCommand::Clear();	
	}

	Hazel::Renderer3D::BeginScene(m_CameraController.GetCamera());

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
		auto pos = m_CameraController.GetCamera().GetPosition() + m_CameraController.GetCamera().GetFront() * 6.0f;
		x = (x / width) * boundsWidth - boundsWidth * 0.5f;
		y = boundsHeight * 0.5f - (y / height) * boundsHeight;
		m_Particle.Position = { x + pos.x, y + pos.y, z + pos.z };
		for (int i = 0; i < 5; i++)
		{
			m_ParticleSystem.Emit(m_Particle);
		}
	}

	{
		PROFILE_SCOPE("ParticleSystem::OnUpdate");
		if (m_EnabledUpdate)
		{
			m_ParticleSystem.OnUpdate(timestep);
		}
	}
	{
		PROFILE_SCOPE("ParticleSystem.OnRender");
		m_ParticleSystem.OnRender(m_CameraController.GetCamera());
	}

	Hazel::Renderer3D::EndScene();
}

void ParticlesLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Background Color", glm::value_ptr(m_BackgroundColor));
	ImGui::ColorEdit4("Birth Color", glm::value_ptr(m_Particle.ColorBegin));
	ImGui::ColorEdit4("Death Color", glm::value_ptr(m_Particle.ColorEnd));
	ImGui::DragFloat("Life Time", &m_Particle.LifeTime, 0.1f, 0.0f, 100.0f);
	ImGui::DragFloat("Velocity", &m_Velocity, 0.1f, 0.0f, 100.0f);
	ImGui::DragFloat("Rotation Velocity", &m_RotationVelocity, 0.0f, 0.0f, 100.0f);
	ImGui::Checkbox("Enabled 3D", &m_Enabled3D);
	ImGui::Checkbox("Enabled Billboarding", &m_EnabledBillboarding);
	ImGui::Checkbox("Enabled Texture", &m_EnabledTexture);
	ImGui::Checkbox("Enabled Update", &m_EnabledUpdate);
	ImGui::Value("FPS", m_FPS);

	// Profiler section
	ImGui::Text("Profiler:");
	for (auto& result : m_ProfileResults)
	{
		char label[50];
		strcpy_s(label, "%.3f ms  ");
		strcat_s(label, result.Name);
		ImGui::Text(label, result.Time);
	}
	m_ProfileResults.clear();

	ImGui::End();
}
