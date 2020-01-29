#include "SwapChain.h"
#include "../RenderSystem.h"

#include <exception>


SwapChain::SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system): m_system(system)
{
	ID3D11Device* device = m_system->m_d3d_device;
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hwnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE;

	HRESULT hr = m_system->m_dxgi_factory->CreateSwapChain(device, &desc, &m_swap_chain);

	if (FAILED(hr))
	{
		throw std::exception("SwapChain initialization failed.");
	}

	ID3D11Texture2D* buffer = NULL;
	hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(hr))
	{
		throw std::exception("SwapChain: GetBuffer failed.");
	}

	hr = device->CreateRenderTargetView(buffer, NULL, &m_rtv);

	if (FAILED(hr))
	{
		throw std::exception("SwapChain: CreateRenderTargetView failed.");
	}

}

bool SwapChain::present(bool vsync)
{
	m_swap_chain->Present(vsync, NULL);
	return true;
}

SwapChain::~SwapChain()
{
	m_rtv->Release();
	m_swap_chain->Release();
}
