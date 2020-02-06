#pragma once

#include "Core.h"
#include "Window.h"
#include "Hazel/Core/LayerStack.h"
#include "Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/ImGui/ImGuiLayer.h"


namespace Hazel
{
	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		float m_LastFrameTime = 0.0f;
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;

	private:

		static Application* s_Instance;

	};

	// To be defined in Client
	Application* CreateApplication();

}
