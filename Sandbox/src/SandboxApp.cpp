#include <Hazel.h>

#include "Hazel/Core.h"

#include "Hazel/Platform/OpenGL/OpenGLShader.h"

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

		float vertices[3 * 3 + 3 * 4] =
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

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = v_Color;
			}
		)";

		m_Shader.reset(Hazel::Shader::Create(vertexSrc, fragmentSrc));

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

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec4 u_Color;

			void main()
			{
				color = u_Color;
			}
		)";

		m_FlatColorShader.reset(Hazel::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));

		std::string textureShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string textureShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;

			uniform vec4 u_Color;

			void main()
			{
				color = vec4(v_TexCoord, 0.0, 1.0);
			}
		)";

		m_TextureShader.reset(Hazel::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));
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

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

		for (int y = -9; y <= 9; y++)
		{
			for (int x = -16; x <= 16; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		Hazel::Renderer::Submit(m_TextureShader, m_SquareVA, glm::translate(glm::mat4(1.0f), m_TrianglePosition) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		// glm::mat4 transformTriangle = glm::translate(glm::mat4(1.0f), m_TrianglePosition);
		// glm::mat4 scaleTriangle = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
		// Hazel::Renderer::Submit(m_Shader, m_VertexArray, transformTriangle * scaleTriangle);

		Hazel::Renderer::EndScene();
	}

private:

	Hazel::Ref<Hazel::Shader> m_Shader;
	Hazel::Ref<Hazel::VertexArray> m_VertexArray;

	Hazel::Ref<Hazel::Shader> m_FlatColorShader;
	Hazel::Ref<Hazel::Shader> m_TextureShader;
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;

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
