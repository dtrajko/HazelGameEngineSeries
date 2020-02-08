#include "Sandbox3D.h"
#include "Hazel/Platform/OpenGL/OpenGLShader.h"
#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Renderer/Texture.h"
#include "Hazel/Loader/Loader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox3D::Sandbox3D()
	: Layer("Sandbox3D"), m_CameraController(16.f / 9.f, true)
{
	m_SquareVA = Hazel::VertexArray::Create();

	float squareVertices[24 * 5] =
	{
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
	};

	Hazel::Ref<Hazel::VertexBuffer> squareVB;
	squareVB = Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout({
		{ Hazel::ShaderDataType::Float3, "a_Position" },
		{ Hazel::ShaderDataType::Float2, "a_TexCoord" },
	});

	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[12 * 3] = {
		 0,  1,  3,
		 3,  1,  2,
		 4,  5,  7,
		 7,  5,  6,
		 8,  9, 11,
		11,  9, 10,
		12, 13, 15,
		15, 13, 14,
		16, 17, 19,
		19, 17, 18,
		20, 21, 23,
		23, 21, 22,
	};

	Hazel::Ref<Hazel::IndexBuffer> squareIB;
	squareIB = Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVA->SetIndexBuffer(squareIB);

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

	// Render
	Hazel::RenderCommand::SetClearColor(m_BackgroundColor);
	Hazel::RenderCommand::Clear();

	Hazel::Renderer::BeginScene(m_CameraController.GetCamera().GetViewProjectionMatrix());

	m_FlatColorShader->Bind();
	m_FlatColorShader->SetFloat4("u_Color", m_SquareColor1);
	Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.0f)));
	m_FlatColorShader->SetFloat4("u_Color", m_SquareColor2);
	Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f)));
	m_FlatColorShader->SetFloat4("u_Color", m_SquareColor3);
	Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f)));

	Hazel::Renderer::EndScene();
}

void Sandbox3D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Sandbox3D BgColor", glm::value_ptr(m_BackgroundColor));
	ImGui::ColorEdit4("Square Color 1", glm::value_ptr(m_SquareColor1));
	ImGui::ColorEdit4("Square Color 2", glm::value_ptr(m_SquareColor2));
	ImGui::ColorEdit4("Square Color 3", glm::value_ptr(m_SquareColor3));
	ImGui::End();
}

void Sandbox3D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}
