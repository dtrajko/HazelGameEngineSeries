#include <Hazel.h>
#include "Hazel/Window.h"

#include "GameEngine/WindowingSystem/Window.h"
#include "GameEngine/GraphicsEngine/RenderSystem/RenderSystem.h"
#include "GameEngine/GraphicsEngine/RenderSystem/SwapChain/SwapChain.h"
#include "GameEngine/GraphicsEngine/RenderSystem/DeviceContext/DeviceContext.h"
#include "GameEngine/GraphicsEngine/RenderSystem/VertexBuffer/VertexBuffer.h"
#include "GameEngine/GraphicsEngine/RenderSystem/IndexBuffer/IndexBuffer.h"
#include "GameEngine/GraphicsEngine/RenderSystem/ConstantBuffer/ConstantBuffer.h"
#include "GameEngine/GraphicsEngine/RenderSystem/VertexShader/VertexShader.h"
#include "GameEngine/GraphicsEngine/RenderSystem/PixelShader/PixelShader.h"
#include "GameEngine/InputSystem/InputListener.h"
#include "GameEngine/Math/Matrix4x4.h"
#include "GameEngine/Math/Point.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>



class DX11Layer : public Hazel::Layer
{

public:

	DX11Layer() : Layer("VulkanLayer")
	{
	
	}

	void OnUpdate() override
	{
		// HZ_INFO("ExampleLayer::Update");
	}

	void OnEvent(Hazel::Event& event) override
	{
		HZ_TRACE("{0}", event);
	}

};

class DX11App : public Hazel::Application
{

private:

	Hazel::Window* window;
	GLFWwindow* windowHandler;

public:


private:
	RenderSystem* m_render_system;
	SwapChain* m_swap_chain;
	VertexBuffer* m_vb;
	IndexBuffer* m_ib;
	ConstantBuffer* m_cb;
	VertexShader* m_vs;
	PixelShader* m_ps;

	// Timer
	float m_old_delta;
	float m_new_delta;
	float m_delta_time;
	float m_delta_pos;
	float m_delta_rot;
	float m_delta_scale;

	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;

	float m_scale_cube = 1;

	float m_forward = 0.0f;
	float m_right = 0.0f;
	float m_up = 0.0f;
	Matrix4x4 m_world_cam;

	float cam_speed = 0.02f;
	float mouse_speed = 0.01f;

	float width;
	float height;

	HWND m_hwnd;
	bool m_is_run;

public:

	DX11App();
	void Create();
	void OnUpdate();
	void Update();
	void Run();
	~DX11App();

private:

	RECT getClientWindowRect();

};

Hazel::Application* Hazel::CreateApplication()
{
	Application::s_Graphics_API = GraphicsAPI::DX11;

	return new DX11App();
}
