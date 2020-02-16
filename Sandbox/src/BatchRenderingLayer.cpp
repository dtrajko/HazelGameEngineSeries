#include "BatchRenderingLayer.h"

#include "Hazel/Renderer/CameraController.h"
#include "Hazel/Core/Timer.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include <stb_image/stb_image.h>


#define PROFILE_SCOPE(name) Hazel::Timer timer##__LINE__(name, [&](BatchRenderingLayer::ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

BatchRenderingLayer::BatchRenderingLayer()
	: Layer("Batch Rendering Layer"), m_CameraController(16.f / 9.f, true)
{
}

void BatchRenderingLayer::OnAttach()
{
	Hazel::RenderCommand::Init();

	m_Shader = Hazel::Ref<Hazel::Shader>(Hazel::Shader::Create(
		"assets/shaders/BatchRendering.glsl.vert",
		"assets/shaders/BatchRendering.glsl.frag"
	));

	glUseProgram(m_Shader->GetRendererID());

	auto loc = glGetUniformLocation(m_Shader->GetRendererID(), "u_Textures");
	int samplers[2] = { 0, 1 };
	glUniform1iv(loc, 2, samplers);

	float vertices[] =
	{
		// X     Y     Z       R      G     B      A       U     V       S
		-1.5f, -0.5f, 0.0f,   0.18f, 0.6f, 0.96f, 1.0f,   0.0f, 0.0f,   0.0f,
		-0.5f, -0.5f, 0.0f,   0.18f, 0.6f, 0.96f, 1.0f,   1.0f, 0.0f,   0.0f,
		-0.5f,  0.5f, 0.0f,   0.18f, 0.6f, 0.96f, 1.0f,   1.0f, 1.0f,   0.0f,
		-1.5f,  0.5f, 0.0f,   0.18f, 0.6f, 0.96f, 1.0f,   0.0f, 1.0f,   0.0f,
																	    
		 0.5f, -0.5f, 0.0f,   1.0f, 0.93f, 0.24f, 1.0f,   0.0f, 0.0f,   1.0f,
		 1.5f, -0.5f, 0.0f,   1.0f, 0.93f, 0.24f, 1.0f,   1.0f, 0.0f,   1.0f,
		 1.5f,  0.5f, 0.0f,   1.0f, 0.93f, 0.24f, 1.0f,   1.0f, 1.0f,   1.0f,
		 0.5f,  0.5f, 0.0f,   1.0f, 0.93f, 0.24f, 1.0f,   0.0f, 1.0f,   1.0f,
	};

	glCreateVertexArrays(1, &m_QuadVA);
	glBindVertexArray(m_QuadVA);

	glCreateBuffers(1, &m_QuadVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(m_QuadVA, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), 0);

	glEnableVertexArrayAttrib(m_QuadVA, 1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)12);

	glEnableVertexArrayAttrib(m_QuadVA, 2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)28);

	glEnableVertexArrayAttrib(m_QuadVA, 3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)36);

	uint32_t indices[] =
	{
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
	};

	glCreateBuffers(1, &m_QuadIB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	m_Texture1 = (GLuint)Hazel::Texture2D::LoadTexture("assets/textures/leather.jpg");
	m_Texture2 = (GLuint)Hazel::Texture2D::LoadTexture("assets/textures/metal.jpg");
}

void BatchRenderingLayer::OnDetach()
{
	// Shutdown here
}

void BatchRenderingLayer::OnEvent(Hazel::Event& event)
{
	m_CameraController.OnEvent(event);
}

static void SetUniformMat4(uint32_t shader, const char* name, const glm::mat4& matrix)
{
	int loc = glGetUniformLocation(shader, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void BatchRenderingLayer::OnUpdate(Hazel::Timestep timestep)
{
	PROFILE_SCOPE("BatchRenderingLayer::OnUpdate");

	// Update
	m_CameraController.OnUpdate(timestep);

	m_FPS = (unsigned int)(1.0f / timestep.GetSeconds());

	Hazel::RenderCommand::SetClearColor(m_BgColor);
	Hazel::RenderCommand::Clear();

	Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

	// Render here
	glUseProgram(m_Shader->GetRendererID());
	glBindTextureUnit(0, m_Texture1);
	glBindTextureUnit(1, m_Texture2);

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));
	if (m_BillboardingEnabled)
	{
		transform = glm::rotate(transform, glm::radians(m_CameraController.GetCamera().GetRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(m_CameraController.GetCamera().GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(m_CameraController.GetCamera().GetRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	auto viewProjection = m_CameraController.GetCamera().GetViewProjectionMatrix();
	SetUniformMat4(m_Shader->GetRendererID(), "u_ViewProj", viewProjection);
	SetUniformMat4(m_Shader->GetRendererID(), "u_Transform", transform);

	glBindVertexArray(m_QuadVA);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);

	Hazel::Renderer::EndScene();
}

void BatchRenderingLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("BgColor: ", glm::value_ptr(m_BgColor));
	ImGui::Checkbox("Billboarding: ", &m_BillboardingEnabled);
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
