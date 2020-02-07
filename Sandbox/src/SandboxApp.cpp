#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "Hazel/Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ExampleLayer.h"
#include "Sandbox2D.h"
#include "Sandbox3D.h"


class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		// PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
		// PushLayer(new Sandbox3D());
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
