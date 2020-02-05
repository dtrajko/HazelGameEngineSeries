#include <Hazel.h>

#include "Hazel/Core.h"
#include "Hazel/Platform/OpenGL/OpenGLShader.h"
#include "Hazel/Renderer/Shader.h"

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>

#include <memory>


class ExampleLayer : public Hazel::Layer
{

public:

	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_TrianglePosition(0.0f)
	{
		if (Hazel::Renderer::GetAPI() != Hazel::RendererAPI::API::OpenGL) return;

		DrawOpenGLTriangle();
	}

	void DrawOpenGLTriangle()
	{
		m_VertexArray.reset(Hazel::VertexArray::Create());

		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};

		Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

		Hazel::BufferLayout layout =
		{
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" },
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		auto m_Shader = m_ShaderLibrary.Load("assets/shaders/Triangle.glsl");

		/* 2nd vertex array and shaders */

		m_SquareVA.reset(Hazel::VertexArray::Create());

		float squareVertices[5 * 4] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};

		Hazel::Ref<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float2, "a_TexCoord" },
		});

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Hazel::Ref<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		auto flatColorShader = m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");

		textureShader->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Hazel::Timestep timestep) override
	{
		if (Hazel::RendererAPI::GetAPI() != Hazel::RendererAPI::API::OpenGL) return;

		// HZ_TRACE("Delta time: {0} sec, {1} ms", timestep.GetSeconds(), timestep.GetMilliseconds());

		m_FPS = (unsigned int)(1.0f / timestep.GetSeconds());

		UpdateInputPolling(timestep);

		Hazel::RenderCommand::SetClearColor(m_BackgroundColor);
		Hazel::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Hazel::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		auto flatColorShader = m_ShaderLibrary.Get("FlatColor");

		flatColorShader->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(flatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

		for (int y = -9; y <= 9; y++)
		{
			for (int x = -16; x <= 16; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Hazel::Renderer::Submit(flatColorShader, m_SquareVA, transform);
			}
		}

		m_Texture->Bind();
		auto m_TextureShader = m_ShaderLibrary.Get("Texture");
		Hazel::Renderer::Submit(m_TextureShader, m_SquareVA,
			glm::translate(glm::mat4(1.0f), m_TrianglePosition) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_ChernoLogoTexture->Bind();
		Hazel::Renderer::Submit(m_TextureShader, m_SquareVA,
			glm::translate(glm::mat4(1.0f), m_TrianglePosition) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Hazel::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Background Color", glm::value_ptr(m_BackgroundColor));
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::Value("FPS", m_FPS);
		ImGui::End();
	}

	void UpdateInputPolling(Hazel::Timestep timestep)
	{
		/* Camera movement Begin */

		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT) || Hazel::Input::IsKeyPressed(HZ_KEY_A))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed * timestep.GetSeconds();
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT) || Hazel::Input::IsKeyPressed(HZ_KEY_D))
		{
			m_CameraPosition.x += m_CameraMoveSpeed * timestep.GetSeconds();
		}

		if (Hazel::Input::IsKeyPressed(HZ_KEY_UP) || Hazel::Input::IsKeyPressed(HZ_KEY_W))
		{
			m_CameraPosition.y += m_CameraMoveSpeed * timestep.GetSeconds();
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN) || Hazel::Input::IsKeyPressed(HZ_KEY_S))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed * timestep.GetSeconds();
		}

		if (Hazel::Input::IsKeyPressed(HZ_KEY_R))
		{
			m_CameraRotation += m_CameraRotationSpeed * timestep.GetSeconds();
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_T))
		{
			m_CameraRotation -= m_CameraRotationSpeed * timestep.GetSeconds();
		}

		/* Camera movement End */

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

	void OnEvent(Hazel::Event& event) override
	{
	}

private:

	Hazel::ShaderLibrary m_ShaderLibrary;

	Hazel::Ref<Hazel::VertexArray> m_VertexArray;
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;

	Hazel::Ref<Hazel::Texture2D> m_Texture;
	Hazel::Ref<Hazel::Texture2D> m_ChernoLogoTexture;

	Hazel::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 2.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 90.0f;

	glm::vec3 m_TrianglePosition;
	float m_SquareMoveSpeed = 1.0f;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec4 m_BackgroundColor = { 0.9f, 0.8f, 0.8f, 1.0f };

	unsigned int m_FPS;

};

/******************** ExampleLayer End ********************/

//******************** Sandbox Begin ********************/

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}

};

Hazel::Application* Hazel::CreateApplication()
{
	RendererAPI::SetAPI(RendererAPI::API::OpenGL);

	return new Sandbox();
}
