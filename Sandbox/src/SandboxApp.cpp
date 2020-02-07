#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "Hazel/Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"


class ExampleLayer : public Hazel::Layer
{

public:

	ExampleLayer()
		: Layer("Example"), m_CameraController(16.0f / 9.0f, true)
	{
		if (Hazel::Renderer::GetAPI() != Hazel::RendererAPI::API::OpenGL) return;

		DrawOpenGLTriangle();
	}

	void DrawOpenGLTriangle()
	{
		m_VertexArray = Hazel::VertexArray::Create();

		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};

		Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer = Hazel::VertexBuffer::Create(vertices, sizeof(vertices));

		Hazel::BufferLayout layout =
		{
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" },
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
		indexBuffer = Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		auto m_Shader = m_ShaderLibrary.Load("assets/shaders/Triangle.glsl");

		/* 2nd vertex array and shaders */

		m_SquareVA = Hazel::VertexArray::Create();

		float squareVertices[5 * 4] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};

		Hazel::Ref<Hazel::VertexBuffer> squareVB;
		squareVB = Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float2, "a_TexCoord" },
		});

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Hazel::Ref<Hazel::IndexBuffer> squareIB;
		squareIB = Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
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

		// Update
		m_CameraController.OnUpdate(timestep);

		// Render
		Hazel::RenderCommand::SetClearColor(m_BackgroundColor);
		Hazel::RenderCommand::Clear();

		// HZ_TRACE("Delta time: {0} sec, {1} ms", timestep.GetSeconds(), timestep.GetMilliseconds());

		m_FPS = (unsigned int)(1.0f / timestep.GetSeconds());

		UpdateInputPolling(timestep);

		Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		auto flatColorShader = m_ShaderLibrary.Get("FlatColor");

		flatColorShader->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(flatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

		for (int y = -12; y <= 12; y++)
		{
			for (int x = -20; x <= 20; x++)
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
		m_CameraController.OnEvent(event);

		if (event.GetEventType() == Hazel::EventType::WindowResize)
		{
			auto& resizeEvent = (Hazel::WindowResizeEvent&)event;

			// TODO: adjust zoom level, aspect ratio, projection matrix...
		}
	}

private:

	Hazel::ShaderLibrary m_ShaderLibrary;

	Hazel::Ref<Hazel::VertexArray> m_VertexArray;
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;

	Hazel::Ref<Hazel::Texture2D> m_Texture;
	Hazel::Ref<Hazel::Texture2D> m_ChernoLogoTexture;

	Hazel::OrthographicCameraController m_CameraController;

	glm::vec3 m_TrianglePosition = glm::vec3(0.0f);
	float m_SquareMoveSpeed = 1.0f;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec4 m_BackgroundColor = { 0.9f, 0.8f, 0.8f, 1.0f };

	unsigned int m_FPS = 0;

};

/******************** ExampleLayer End ********************/

//******************** Sandbox Begin ********************/

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		// PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
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
