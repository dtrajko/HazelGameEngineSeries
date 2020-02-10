#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "ExampleLayer.h"
#include "Sandbox2D.h"
#include "Sandbox3D.h"
#include "ParticlesLayer.h"


class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		// PushLayer(new ExampleLayer());
		// PushLayer(new Sandbox2D());
		// PushLayer(new Sandbox3D());
		PushLayer(new ParticlesLayer());
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
