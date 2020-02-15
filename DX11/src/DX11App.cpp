#include "DX11App.h"

#include "Hazel/Core/Core.h"
#include "Hazel/Core/Input.h"

#include "GameEngine/Math/Vector3D.h"
#include "GameEngine/Math/Matrix4x4.h"
#include "GameEngine/InputSystem/InputSystem.h"
#include "GameEngine/GraphicsEngine/GraphicsEngine.h"

#include <iostream>
#include <Windows.h>
#include <exception>


struct vertex
{
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;
};

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)


DX11Layer::DX11Layer()
	: Layer("DirectX 11 Layer"), m_CameraController(16.f / 9.f, true)
{
}

void DX11Layer::OnAttach()
{
	window = &(Hazel::Application::Get().GetWindow());
	windowHandler = (GLFWwindow*)window->GetNativeWindow();
	m_hwnd = glfwGetWin32Window(windowHandler);

	Create();
}

void DX11Layer::OnDetach()
{
}

void DX11Layer::OnEvent(Hazel::Event& event)
{
	m_CameraController.OnEvent(event);

	Hazel::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Hazel::WindowResizeEvent>(HZ_BIND_EVENT_FN(DX11Layer::OnWindowResizeEvent));
}

bool DX11Layer::OnWindowResizeEvent(Hazel::WindowResizeEvent& event)
{
	m_render_system = GraphicsEngine::get()->getRenderSystem();

	m_swap_chain = m_render_system->createSwapChain(this->m_hwnd, event.GetWidth(), event.GetHeight());

	width = (float)event.GetWidth();
	height = (float)event.GetHeight();

	return false;
}

void DX11Layer::Create()
{
	m_render_system = GraphicsEngine::get()->getRenderSystem();

	m_swap_chain = m_render_system->createSwapChain(this->m_hwnd,
		Hazel::Application::Get().GetWindow().GetWidth(),
		Hazel::Application::Get().GetWindow().GetHeight());

	HZ_ASSERT(m_swap_chain, "Swap chain create failed!");

	width = (float)Hazel::Application::Get().GetWindow().GetWidth();
	height = (float)Hazel::Application::Get().GetWindow().GetHeight();

	HZ_INFO("Display width: {0}", Hazel::Application::Get().GetWindow().GetWidth());
	HZ_INFO("Display height: {0}", Hazel::Application::Get().GetWindow().GetHeight());

	vertex vertex_list[] =
	{
		//          POSITION                      COLOR                       COLOR
		//          X   -  Y  -  Z                R  -  G  -  B               R  -  G  -  B
		// Front face
		{ Vector3D(-0.5f, -0.5f, -0.5f), Vector3D(1.0f, 0.0f, 0.0f), Vector3D(1.0f, 0.0f, 0.0f) }, // POS0
		{ Vector3D(-0.5f,  0.5f, -0.5f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f) }, // POS1
		{ Vector3D(0.5f,  0.5f, -0.5f), Vector3D(0.0f, 0.0f, 1.0f), Vector3D(0.0f, 0.0f, 1.0f) }, // POS2
		{ Vector3D(0.5f, -0.5f, -0.5f), Vector3D(1.0f, 1.0f, 0.0f), Vector3D(1.0f, 1.0f, 0.0f) }, // POS3

		// Back face
		{ Vector3D(0.5f, -0.5f,  0.5f), Vector3D(1.0f, 1.0f, 0.0f), Vector3D(1.0f, 0.0f, 0.0f) }, // POS4
		{ Vector3D(0.5f,  0.5f,  0.5f), Vector3D(0.0f, 0.0f, 1.0f), Vector3D(0.0f, 1.0f, 0.0f) }, // POS5
		{ Vector3D(-0.5f,  0.5f,  0.5f), Vector3D(0.0f, 1.0f, 0.0f), Vector3D(0.0f, 0.0f, 1.0f) }, // POS6
		{ Vector3D(-0.5f, -0.5f,  0.5f), Vector3D(1.0f, 0.0f, 0.0f), Vector3D(1.0f, 1.0f, 0.0f) }, // POS7
	};

	unsigned int index_list[] =
	{
		// Front side
		0, 1, 2, // First triangle
		2, 3, 0, // Second triangle
		// Back side
		4, 5, 6,
		6, 7, 4,
		// Top side
		1, 6, 5,
		5, 2, 1,
		// Bottom side
		7, 0, 3,
		3, 4, 7,
		// Right side
		3, 2, 5,
		5, 4, 3,
		// Left side
		7, 6, 1,
		1, 0, 7,
	};

	// Vertex buffer
	UINT size_vertex_list = ARRAYSIZE(vertex_list);
	void* shader_byte_code = nullptr;
	UINT size_shader = 0;

	// Vertex shader
	m_render_system->compileVertexShader(L"shaders/VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vb = m_render_system->createVertexBuffer(vertex_list, sizeof(vertex), size_vertex_list, shader_byte_code, size_shader);
	m_vs = m_render_system->createVertexShader(shader_byte_code, size_shader);
	m_render_system->releaseCompiledShader();

	// Pixel shader
	m_render_system->compilePixelShader(L"shaders/PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = m_render_system->createPixelShader(shader_byte_code, size_shader);
	m_render_system->releaseCompiledShader();

	// Index buffer
	UINT size_index_list = ARRAYSIZE(index_list);
	m_ib = m_render_system->createIndexBuffer(index_list, size_index_list);

	// Constant buffer
	constant cc;
	cc.m_time = 0;
	m_cb = m_render_system->createConstantBuffer(&cc, sizeof(constant));
}

void DX11Layer::OnUpdate(Hazel::Timestep timestep)
{
	// Update
	m_CameraController.OnUpdate(timestep);

	// Clear the render target
	m_render_system->getImmediateDeviceContext()->clearRenderTargetColor(m_swap_chain, 0.2f, 0.0f, 0.2f, 1);

	// Set Viewport of render target in which we have to draw
	m_render_system->getImmediateDeviceContext()->setViewportSize(
		Hazel::Application::Get().GetWindow().GetWidth(),
		Hazel::Application::Get().GetWindow().GetHeight());

	this->UpdateScene();

	m_render_system->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	m_render_system->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	// Set default shader in the graphics pipeline to be able to draw 
	m_render_system->getImmediateDeviceContext()->setVertexShader(m_vs);
	m_render_system->getImmediateDeviceContext()->setPixelShader(m_ps);

	// Set the vertices of the triangle to draw
	m_render_system->getImmediateDeviceContext()->setVertexBuffer(m_vb);

	// Set index buffer
	m_render_system->getImmediateDeviceContext()->setIndexBuffer(m_ib);

	// Finally draw the triangle
	m_render_system->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);

	m_swap_chain->present(true);
}

void DX11Layer::UpdateScene()
{
	constant cc;
	cc.m_time = (unsigned int)::GetTickCount64();

	cc.m_world.setIdentity();
	cc.m_view.setIdentity();
	cc.m_proj.setIdentity();

	Matrix4x4 temp;
	temp.setIdentity();
	temp.setTranslation(Vector3D(0.0f, 0.0f, -10.0f));
	cc.m_world *= temp;

	cc.m_view = Matrix4x4(m_CameraController.GetCamera().GetViewMatrix());
	cc.m_proj = Matrix4x4(m_CameraController.GetCamera().GetProjectionMatrix());

	glm::vec3 camPos = m_CameraController.GetCamera().GetRight();
	HZ_TRACE("Camera Right Vector X: {0}, Y: {1}, Z: {2}", camPos.x, camPos.y, camPos.z);

	m_cb->update(m_render_system->getImmediateDeviceContext(), &cc);
}

DX11Layer::~DX11Layer()
{
	delete m_ps;
	delete m_vs;
	delete m_cb;
	delete m_ib;
	delete m_vb;
	delete m_swap_chain;
}


DX11App::DX11App()
{
	PushLayer(new DX11Layer());
}

DX11App::~DX11App()
{
}
