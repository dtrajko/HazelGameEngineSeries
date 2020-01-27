#include <Hazel.h>


class ExampleLayer : public Hazel::Layer
{

public:

	ExampleLayer() : Layer("Example")
	{
	
	}

	void OnUpdate() override
	{
		if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
		{
			HZ_INFO("TAB key is pressed!");
		}
	}

	void OnEvent(Hazel::Event& event) override
	{
		// HZ_TRACE("{0}", event);
	}

};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		// Hazel::Log::GetCoreLogger()->warn("Core Logger initialized!");
		// Hazel::Log::GetClientLogger()->info("Client Logger initialized!");
		HZ_CORE_WARN("Core Logger (Macro) initialized!");
		int a = 5;
		HZ_INFO("Client Logger (Macro) initialized! Var={0}", a);

		PushLayer(new ExampleLayer());
		PushOverlay(new Hazel::ImGuiLayer());
	}

	~Sandbox()
	{
		
	}

};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}
