#include <Hazel.h>

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
	}

	~Sandbox()
	{
		
	}

};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}
