#include "Sandbox3D.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Hazel/Models/Cube.h"


Sandbox3D::Sandbox3D()
	: Layer("Sandbox3D"), m_CameraController(16.f / 9.f, true)
{
	m_CubeVA = Hazel::VertexArray::Create();

	Hazel::Ref<Hazel::VertexBuffer> cubeVB;
	cubeVB = Hazel::VertexBuffer::Create(Hazel::Cube::vertices , sizeof(Hazel::Cube::vertices));
	cubeVB->SetLayout({
		{ Hazel::ShaderDataType::Float3, "a_Position" },
		{ Hazel::ShaderDataType::Float2, "a_TexCoord" },
	});

	m_CubeVA->AddVertexBuffer(cubeVB);

	Hazel::Ref<Hazel::IndexBuffer> cubeIB;
	cubeIB = Hazel::IndexBuffer::Create(Hazel::Cube::indices, sizeof(Hazel::Cube::indices) / sizeof(uint32_t));
	m_CubeVA->SetIndexBuffer(cubeIB);

	m_FlatColorShader = Hazel::Shader::Create("assets/shaders/Sandbox3D.glsl");
	m_Texture = Hazel::Texture2D::Create("assets/textures/statue_512x512.jpg");
	m_FlatColorShader->Bind();
	m_FlatColorShader->SetInt("u_Texture", 0);
}

void Sandbox3D::OnAttach()
{
}

void Sandbox3D::OnDetach()
{
}

void Sandbox3D::OnUpdate(Hazel::Timestep timestep)
{
	// Update
	m_CameraController.OnUpdate(timestep);
	m_CameraController.GetCamera().SetFOV(m_FOV);

	HZ_TRACE("FOV: {0}", m_FOV);

	// Render
	Hazel::RenderCommand::SetClearColor(m_BackgroundColor);
	Hazel::RenderCommand::Clear();

	Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

	m_FlatColorShader->Bind();
	m_FlatColorShader->SetFloat4("u_Color", m_CubeColor1);
	Hazel::Renderer::Submit(m_FlatColorShader, m_CubeVA, m_CameraController.GetCamera().GetViewProjectionMatrix(),
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.0f)));
	m_FlatColorShader->SetFloat4("u_Color", m_CubeColor2);
	Hazel::Renderer::Submit(m_FlatColorShader, m_CubeVA, m_CameraController.GetCamera().GetViewProjectionMatrix(),
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f)));
	m_FlatColorShader->SetFloat4("u_Color", m_CubeColor3);
	Hazel::Renderer::Submit(m_FlatColorShader, m_CubeVA, m_CameraController.GetCamera().GetViewProjectionMatrix(),
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f)));

	Hazel::Renderer::DrawCube({ 2.0f, 0.0f, -2.0f }, { 1.0f, 1.0f, 1.0f }, m_Texture);
	Hazel::Renderer::DrawCube({ 2.0f, 0.0f, -4.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.6f });
	Hazel::Renderer::DrawCube({ 2.0f, 1.0f, -4.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.6f });
	Hazel::Renderer::DrawCube({ 2.0f, 2.0f, -4.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 0.6f });

	Hazel::Renderer::EndScene();
}

void Sandbox3D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Sandbox3D BgColor", glm::value_ptr(m_BackgroundColor));
	ImGui::ColorEdit4("Cube Color 1", glm::value_ptr(m_CubeColor1));
	ImGui::ColorEdit4("Cube Color 2", glm::value_ptr(m_CubeColor2));
	ImGui::ColorEdit4("Cube Color 3", glm::value_ptr(m_CubeColor3));
	ImGui::DragFloat("FOV", &m_FOV, 0.001f, glm::radians(4.0f), glm::radians(160.0f));
	ImGui::End();
}

void Sandbox3D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}
