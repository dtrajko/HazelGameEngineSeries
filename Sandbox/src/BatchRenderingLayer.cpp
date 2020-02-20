#include "BatchRenderingLayer.h"

#include "Hazel/Renderer/CameraController.h"
#include "Hazel/Core/Timer.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include <stb_image/stb_image.h>


#define PROFILE_SCOPE(name) Hazel::Timer timer##__LINE__(name, [&](BatchRenderingLayer::ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })


struct Vec2 { float x, y; };
struct Vec3 { float x, y, z; };
struct Vec4 { float x, y, z, w; };

struct Vertex
{
	Vec3 Position;
	Vec4 Color;
	Vec2 TexCoords;
	float TexID;
};


static Vertex* CreateQuad(Vertex* target, float x, float y, float textureID)
{
	float size = 1.0f;

	target->Position = { x, y, 0.0f };
	target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
	target->TexCoords = { 0.0f, 0.0f };
	target->TexID = textureID;
	target++;

	target->Position = { x + size, y, 0.0f };
	target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
	target->TexCoords = { 1.0f, 0.0f };
	target->TexID = textureID;
	target++;

	target->Position = { x + size, y + size, 0.0f };
	target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
	target->TexCoords = { 1.0f, 1.0f };
	target->TexID = textureID;
	target++;

	target->Position = { x, y + size, 0.0f };
	target->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
	target->TexCoords = { 0.0f, 1.0f };
	target->TexID = textureID;
	target++;

	return target;
}

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

	glClearColor(0.2f, 0.0f, 0.2f, 1.0f);

	const size_t MaxQuadCount = 1000;
	const size_t MaxVertexCount = MaxQuadCount * 4;
	const size_t MaxIndexCount = MaxQuadCount * 6;

	glCreateVertexArrays(1, &m_QuadVA);
	glBindVertexArray(m_QuadVA);

	glCreateBuffers(1, &m_QuadVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexArrayAttrib(m_QuadVA, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));

	glEnableVertexArrayAttrib(m_QuadVA, 1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));

	glEnableVertexArrayAttrib(m_QuadVA, 2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));

	glEnableVertexArrayAttrib(m_QuadVA, 3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID));

	uint32_t indices[MaxIndexCount];
	uint32_t offset = 0;
	for (size_t i = 0; i < MaxIndexCount; i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;

		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;

		offset += 4;
	}

	HZ_TRACE("indices: {0}", sizeof(indices));

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

	// Set dynamic vertex buffer

	uint32_t indexCount = 0;
	std::array<Vertex, 1000> vertices;
	Vertex* buffer = vertices.data();
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			buffer = CreateQuad(buffer, x, y, (x + y) % 2);
			indexCount += 6;
		}
	}

	buffer = CreateQuad(buffer, m_QuadPosition[0], m_QuadPosition[1], 1.0f);
	indexCount += 6;

	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

	Hazel::RenderCommand::SetClearColor(m_BgColor);
	Hazel::RenderCommand::Clear();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

	Hazel::Renderer::EndScene();
}

void BatchRenderingLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("BgColor: ", glm::value_ptr(m_BgColor));
	ImGui::Checkbox("Billboarding: ", &m_BillboardingEnabled);
	ImGui::Value("FPS", m_FPS);
	ImGui::DragFloat2("Quad Position", m_QuadPosition, 0.1f);

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
