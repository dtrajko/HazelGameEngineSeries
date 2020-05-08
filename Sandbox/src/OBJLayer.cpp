#include "OBJLayer.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Glad/include/glad/glad.h>

#include "Hazel/Core/Timer.h"
#include "Hazel/Models/Cube.h"
#include "Hazel/Loader/OBJLoader.h"
#include "Hazel/Renderer/CameraController.h"
#include "Hazel/Renderer/Renderer3D.h"


#define PROFILE_SCOPE(name) Hazel::Timer timer##__LINE__(name, [&](OBJLayer::ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

OBJLayer::OBJLayer()
	: Layer("OBJ Layer"), m_CameraController(16.f / 9.f, true)
{
	Hazel::RendererAPI::SetMode(Hazel::RendererAPI::Mode::Renderer3D);
}

void OBJLayer::OnAttach()
{
	Hazel::RenderCommand::Init();

	OBJLoader::loadOBJModel("assets/models/monkey3.obj");

	m_ModelVA = Hazel::VertexArray::Create();
	Hazel::Ref<Hazel::VertexBuffer> modelVB;

	modelVB = Hazel::VertexBuffer::Create(OBJLoader::m_VerticesArray, OBJLoader::m_VerticesCount);


	modelVB->SetLayout({
		{ Hazel::ShaderDataType::Float3, "a_Position" },
	});

	m_ModelVA->AddVertexBuffer(modelVB);

	Hazel::Ref<Hazel::IndexBuffer> modelIB;

	modelIB = Hazel::IndexBuffer::Create(OBJLoader::m_IndicesArray, OBJLoader::m_IndicesCount);

	HZ_TRACE("OBJLoader::m_VerticesCount {0} {1}", sizeof(OBJLoader::m_VerticesArray), OBJLoader::m_VerticesCount);
	HZ_TRACE("OBJLoader::indicesCount {0} {1}", sizeof(OBJLoader::m_IndicesArray), OBJLoader::m_IndicesCount);

	m_ModelVA->SetIndexBuffer(modelIB);

	m_ModelShader = Hazel::Shader::Create("assets/shaders/OBJLoader.glsl");
	m_ModelShader->Bind();
	m_ModelShader->SetFloat4("u_Color", glm::vec4(1.0f));
	m_ModelShader->SetFloat3("u_LightDirection", { -10.0f, 10.0f, 10.0f });

	m_ModelTexture = Hazel::Texture2D::Create("assets/textures/bricks.jpg");
	m_ModelTexture->Bind();
	m_ModelShader->SetInt("u_Texture", 0);
}

void OBJLayer::OnDetach()
{
	// Shutdown here
}

void OBJLayer::OnEvent(Hazel::Event& event)
{
	m_CameraController.OnEvent(event);
}

void OBJLayer::OnUpdate(Hazel::Timestep timestep)
{
	PROFILE_SCOPE("OBJLayer::OnUpdate");

	// Update
	m_CameraController.OnUpdate(timestep);

	m_FPS = (unsigned int)(1.0f / timestep.GetSeconds());

	m_ModelRotation.y -= 1.0f * timestep;

	// Render
	{
		PROFILE_SCOPE("RenderCommand::SetClearColor");
		Hazel::RenderCommand::SetClearColor(m_BackgroundColor);
		Hazel::RenderCommand::Clear();	
	}

	Hazel::Renderer3D::BeginScene(m_CameraController.GetCamera());

	// Render here
	m_ModelShader->Bind();
	m_ModelTexture->Bind();

	Hazel::Renderer3D::Submit(m_ModelShader, m_ModelVA, m_CameraController.GetCamera().GetViewProjectionMatrix(),
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f)) *
		glm::rotate(glm::mat4(1.0f), m_ModelRotation.y, { 0.0f, 1.0f, 0.0f }) *
		glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));

	m_ModelTexture->Unbind();

	Hazel::Renderer3D::EndScene();
}

void OBJLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Background Color", glm::value_ptr(m_BackgroundColor));
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
