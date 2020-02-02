#include <Hazel.h>

#include "Hazel/Core.h"

#include "imgui/imgui.h"


class ExampleLayer : public Hazel::Layer
{

public:

	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_SquarePosition(0.0f)
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

		std::shared_ptr<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

		Hazel::BufferLayout layout =
		{
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" },
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Hazel::IndexBuffer> indexBuffer;
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

		m_Shader.reset(new Hazel::Shader(vertexSrc, fragmentSrc));

		/* 2nd vertex array and shaders */

		m_SquareVA.reset(Hazel::VertexArray::Create());

		float squareVertices[3 * 4] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
		};

		std::shared_ptr<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			});

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Hazel::IndexBuffer> squareIB;
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

		m_FlatColorShader.reset(new Hazel::Shader(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));
	}

	virtual void OnImGuiRender() override
	{
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

		if (Hazel::Input::IsKeyPressed(HZ_KEY_1))
		{
			m_CameraRotation += m_CameraRotationSpeed * timestep.GetSeconds();
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_2))
		{
			m_CameraRotation -= m_CameraRotationSpeed * timestep.GetSeconds();
		}

		/* Camera movement End */

		/* Model movement Begin */

		if (Hazel::Input::IsKeyPressed(HZ_KEY_J))
		{
			m_SquarePosition.x -= m_SquareMoveSpeed * timestep.GetSeconds();
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_L))
		{
			m_SquarePosition.x += m_SquareMoveSpeed * timestep.GetSeconds();
		}

		if (Hazel::Input::IsKeyPressed(HZ_KEY_I))
		{
			m_SquarePosition.y += m_SquareMoveSpeed * timestep.GetSeconds();
		}
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_K))
		{
			m_SquarePosition.y -= m_SquareMoveSpeed * timestep.GetSeconds();
		}

		/* Model movement End */
	}

	void OnEvent(Hazel::Event& event) override
	{
	}

	void OnUpdate(Hazel::Timestep timestep) override
	{
		if (Hazel::RendererAPI::GetAPI() != Hazel::RendererAPI::API::OpenGL) return;

		HZ_TRACE("Delta time: {0} sec, {1} ms", timestep.GetSeconds(), timestep.GetMilliseconds());

		UpdateInputPolling(timestep);

		Hazel::RenderCommand::SetClearColor({ 0.9f, 0.8f, 0.8f, 1.0f });
		Hazel::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Hazel::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		glm::vec4 redColor(  20.f / 255.f,  61.f / 255.f, 89.f / 255.f, 0.1f); // 143D59  20, 61,89
		glm::vec4 blueColor(244.f / 255.f, 180.f / 255.f, 26.f / 255.f, 0.1f); // c 244,180,26

		for (int y = -9; y <= 9; y++)
		{
			for (int x = -16; x <= 16; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				if ((x + y) % 2 == 0)
					m_FlatColorShader->UploadUniformFloat4("u_Color", redColor);
				else
					m_FlatColorShader->UploadUniformFloat4("u_Color", blueColor);

				Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		glm::mat4 transformTriangle = glm::translate(glm::mat4(1.0f), m_SquarePosition);
		glm::mat4 scaleTriangle = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
		Hazel::Renderer::Submit(m_Shader, m_VertexArray, transformTriangle * scaleTriangle);

		Hazel::Renderer::EndScene();
	}

private:

	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;

	std::shared_ptr<Hazel::Shader> m_FlatColorShader;
	std::shared_ptr<Hazel::VertexArray> m_SquareVA;

	Hazel::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 2.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 90.0f;

	glm::vec3 m_SquarePosition;
	glm::vec3 m_SquareRotation;
	glm::vec3 m_SquareScale;
	float m_SquareMoveSpeed = 1.0f;

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
