// Copyright (c) 2019  PardCode.
// All rights reserved.
//
// This file is part of CPP-3D-Game-Tutorial-Series Project, accessible from https://github.com/PardCode/CPP-3D-Game-Tutorial-Series
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License 
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#include "RenderSystem.h"
#include "../../GraphicsEngine/RenderSystem/SwapChain/SwapChain.h"
#include "../../GraphicsEngine/RenderSystem/DeviceContext/DeviceContext.h"
#include "../../GraphicsEngine/RenderSystem/VertexBuffer/VertexBuffer.h"
#include "../../GraphicsEngine/RenderSystem/IndexBuffer/IndexBuffer.h"
#include "../../GraphicsEngine/RenderSystem/ConstantBuffer/ConstantBuffer.h"
#include "../../GraphicsEngine/RenderSystem/VertexShader/VertexShader.h"
#include "../../GraphicsEngine/RenderSystem/PixelShader/PixelShader.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <exception>
#include <iostream>


RenderSystem::RenderSystem()
{
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	HRESULT res = 0;
	for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
	{
		res = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels, num_feature_levels,
			D3D11_SDK_VERSION, &m_d3d_device, &m_feature_level, &m_imm_context);

		if (SUCCEEDED(res))
		{
			break;
			++driver_type_index;
		}
	}

	if (FAILED(res))
	{
		throw std::exception("RenderSystem: D3D11CreateDevice failed.");
	}

	m_imm_device_context = new DeviceContext(m_imm_context, this);

	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);

	DXGI_ADAPTER_DESC adapterDescription;
	m_dxgi_adapter->GetDesc(&adapterDescription);

	std::cout << "DirectX 11 Info:" << std::endl;
	std::cout << "   Vendor: " << adapterDescription.VendorId << std::endl;
	std::cout << "   Renderer: " << adapterDescription.Description << std::endl;
	std::cout << "   Version: " << adapterDescription.Revision << std::endl;
}

SwapChain* RenderSystem::createSwapChain(HWND hwnd, UINT width, UINT height)
{
	SwapChain* sc = nullptr;
	try
	{
		sc = new SwapChain(hwnd, width, height, this);
	}
	catch (const std::exception&)
	{
		throw std::exception("SwapChain initialization failed.");
	}
	return sc;
}

DeviceContext* RenderSystem::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}

VertexBuffer* RenderSystem::createVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader)
{
	VertexBuffer * vb = nullptr;
	try
	{
		vb = new VertexBuffer(list_vertices, size_vertex, size_list, shader_byte_code, size_byte_shader, this);
	}
	catch (const std::exception&)
	{
		throw std::exception("VertexBuffer initialization failed.");
	}
	return vb;
}

IndexBuffer* RenderSystem::createIndexBuffer(void* list_indices, UINT size_list)
{
	IndexBuffer* ib = nullptr;
	try
	{
		ib = new IndexBuffer(list_indices, size_list, this);
	}
	catch (const std::exception&)
	{
		throw std::exception("IndexBuffer initialization failed.");
	}
	return ib;
}

ConstantBuffer* RenderSystem::createConstantBuffer(void* buffer, UINT size_buffer)
{
	ConstantBuffer* cb = nullptr;
	try
	{
		cb = new ConstantBuffer(buffer, size_buffer, this);
	}
	catch (const std::exception&)
	{
		throw std::exception("ConstantBuffer initialization failed.");
	}
	return cb;
}

VertexShader* RenderSystem::createVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
	VertexShader* vs = nullptr;
	try
	{
		vs = new VertexShader(shader_byte_code, byte_code_size, this);
	}
	catch (const std::exception&)
	{
		throw std::exception("VertexShader initialization failed.");
	}
	return vs;
}

PixelShader* RenderSystem::createPixelShader(const void* shader_byte_code, size_t byte_code_size)
{
	PixelShader* ps = nullptr;
	try
	{
		ps = new PixelShader(shader_byte_code, byte_code_size, this);
	}
	catch (const std::exception&)
	{
		throw std::exception("PixelShader initialization failed.");
	}
	return ps;
}

bool RenderSystem::compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, UINT* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;
	HRESULT hr = D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &m_blob, &error_blob);
	if (FAILED(hr))
	{
		if (error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = (UINT)m_blob->GetBufferSize();

	return true;
}

bool RenderSystem::compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, UINT* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;
	HRESULT hr = D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &m_blob, &error_blob);
	if (FAILED(hr))
	{
		if (error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = (UINT)m_blob->GetBufferSize();

	return true;
}

void RenderSystem::releaseCompiledShader()
{
	if (m_blob) m_blob->Release();
}

void RenderSystem::release()
{
	m_dxgi_device->Release();
	m_dxgi_factory->Release();
	m_dxgi_adapter->Release();
	m_d3d_device->Release();
	m_imm_context->Release();
}

RenderSystem::~RenderSystem()
{
	delete m_imm_device_context;
}
