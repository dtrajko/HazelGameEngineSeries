#pragma once

#include "Core.h"
#include "Window.h"
#include "Hazel/LayerStack.h"
#include "Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"

#include "Hazel/ImGui/ImGuiLayer.h"

#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/OrthographicCamera.h"



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
		void updateInput();

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVA;

		OrthographicCamera m_Camera;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraSpeed = 0.02f;

	private:

		static Application* s_Instance;

	};

	// To be defined in Client
	Application* CreateApplication();

}
