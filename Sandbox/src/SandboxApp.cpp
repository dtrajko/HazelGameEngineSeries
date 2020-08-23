#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "ExampleLayer.h"
#include "Sandbox2D.h"
#include "Sandbox3D.h"
#include "ParticlesLayer.h"
#include "OBJLayer.h"
#include "BatchRenderingLayer.h"
#include "Hazel/Renderer/RendererAPI.h"


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
