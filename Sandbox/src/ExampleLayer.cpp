#include "ExampleLayer.h"

#include "imgui/imgui.h"

#include <glm/gtc/type_ptr.hpp>


ExampleLayer::ExampleLayer()
	: Layer("Example"), m_CameraController(16.0f / 9.0f, true)
{
	if (Hazel::Renderer2D::GetAPI() != Hazel::RendererAPI::API::OpenGL) return;

	Hazel::RendererAPI::SetMode(Hazel::RendererAPI::Mode::Renderer2D);

	DrawOpenGLTriangle();
}

void ExampleLayer::OnAttach()
{
}

void ExampleLayer::OnDetach()
{
}

void ExampleLayer::DrawOpenGLTriangle()
{
	m_Texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
	m_ChernoLogoTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");
}

void ExampleLayer::OnUpdate(Hazel::Timestep timestep)
{
	if (Hazel::RendererAPI::GetAPI() != Hazel::RendererAPI::API::OpenGL) return;

	m_FPS = (unsigned int)(1.0f / timestep.GetSeconds());

	// Update
	m_CameraController.OnUpdate(timestep);

	// Render
	Hazel::RenderCommand::SetClearColor(m_BackgroundColor);
	Hazel::RenderCommand::Clear();


	UpdateInputPolling(timestep);

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

	for (int y = -12; y <= 12; y++)
	{
		for (int x = -20; x <= 20; x++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			Hazel::Renderer2D::DrawQuad(pos, glm::vec2(0.1f), m_SquareColor);
		}
	}

	Hazel::Renderer2D::DrawQuad({m_TrianglePosition.x, m_TrianglePosition.y, 0.1f }, glm::vec3(1.5f), m_Texture);
	Hazel::Renderer2D::DrawQuad({ m_TrianglePosition.x, m_TrianglePosition.y, 0.2f }, glm::vec3(1.5f), m_ChernoLogoTexture);

	Hazel::Renderer2D::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Background Color", glm::value_ptr(m_BackgroundColor));
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::Value("FPS", m_FPS);
	ImGui::End();
}

void ExampleLayer::UpdateInputPolling(Hazel::Timestep timestep)
{
	/* Model movement Begin */

	if (Hazel::Input::IsKeyPressed(HZ_KEY_J))
	{
		m_TrianglePosition.x -= m_SquareMoveSpeed * timestep.GetSeconds();
	}
	else if (Hazel::Input::IsKeyPressed(HZ_KEY_L))
	{
		m_TrianglePosition.x += m_SquareMoveSpeed * timestep.GetSeconds();
	}

	if (Hazel::Input::IsKeyPressed(HZ_KEY_I))
	{
		m_TrianglePosition.y += m_SquareMoveSpeed * timestep.GetSeconds();
	}
	else if (Hazel::Input::IsKeyPressed(HZ_KEY_K))
	{
		m_TrianglePosition.y -= m_SquareMoveSpeed * timestep.GetSeconds();
	}

	/* Model movement End */
}

void ExampleLayer::OnEvent(Hazel::Event& event)
{
	m_CameraController.OnEvent(event);

	if (event.GetEventType() == Hazel::EventType::WindowResize)
	{
		auto& resizeEvent = (Hazel::WindowResizeEvent&)event;

		// TODO: adjust zoom level, aspect ratio, projection matrix...
	}
}
