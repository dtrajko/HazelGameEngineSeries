#include "Sandbox2D.h"
#include "Hazel/Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(16.f / 9.f, true)
{
}

void Sandbox2D::OnAttach()
{
	m_BackgroundTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
	m_QuadTexture = Hazel::Texture2D::Create("assets/textures/statue_512x512.jpg");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Hazel::Timestep timestep)
{
	HZ_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(timestep);

	// Render
	{
		HZ_PROFILE_SCOPE("Renderer Prep");
		Hazel::RenderCommand::SetClearColor(m_BackgroundColor);
		Hazel::RenderCommand::Clear();
	}

	{
		HZ_PROFILE_SCOPE("Renderer Draw");
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Hazel::Renderer2D::DrawQuad({ -1.0f,  0.0f, 0.0f }, { 0.8f, 0.8f  }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Hazel::Renderer2D::DrawQuad({  0.5f, -0.5f, 0.0f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });

		// Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 10.0f, 10.0f }, m_BackgroundTexture);
		// Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.1f }, { 2.0f, 1.0f }, m_QuadColor);
		// Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.2f }, { 1.0f, 2.0f }, m_QuadTexture);

		Hazel::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Background Color", glm::value_ptr(m_BackgroundColor));
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_QuadColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}
