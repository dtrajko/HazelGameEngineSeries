#pragma once

#include "Core.h"
#include "Window.h"
#include "Hazel/LayerStack.h"
#include "Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"

#include "Hazel/ImGui/ImGuiLayer.h"

#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Buffer.h"


enum class GraphicsAPI
{
	OPENGL,
	VULKAN,
	DX11,
};

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

		// temporary
		void DrawOpenGLTriangle();

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray;

		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

	private:

		static Application* s_Instance;

	public:

		static GraphicsAPI s_Graphics_API;

	};

	// To be defined in Client
	Application* CreateApplication();

}
