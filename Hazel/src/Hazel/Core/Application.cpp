#include "hzpch.h"

#include "Application.h"
#include "Hazel/Core/Log.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Renderer/Renderer2D.h"
#include "Hazel/Renderer/Renderer3D.h"
#include "Hazel/Core/KeyCodes.h"

#include <GLFW/glfw3.h>


namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
		{
			m_ImGuiLayer = new Hazel::ImGuiLayer();
			PushOverlay(m_ImGuiLayer);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			HZ_TRACE(e);
		}

		if (e.IsInCategory(EventCategoryInput))
		{
			HZ_TRACE(e);
		}

		if (RendererAPI::GetMode() == RendererAPI::Mode::Renderer2D)
		{
			Renderer2D::Init();
		}

		if (RendererAPI::GetMode() == RendererAPI::Mode::Renderer3D)
		{
			Renderer3D::Init();
		}

		while (m_Running)
		{
			float time = (float)glfwGetTime(); // Platform::GetTime()
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			auto [x, y] = Input::GetMousePosition();

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
				{
					layer->OnUpdate(timestep);
				}
			}

			if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
			{
				m_ImGuiLayer->Begin();
				for (Layer* layer : m_LayerStack)
				{
					layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	Application::~Application()
	{
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		HZ_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;

		if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
		{
			if (RendererAPI::GetMode() == RendererAPI::Mode::Renderer2D)
			{
				Renderer2D::OnWindowResize(e.GetWidth(), e.GetHeight());
			}
			if (RendererAPI::GetMode() == RendererAPI::Mode::Renderer3D)
			{
				Renderer3D::OnWindowResize(e.GetWidth(), e.GetHeight());
			}
		}

		return false;
	}

}
