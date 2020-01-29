#include "IndexBuffer.h"
#include "../RenderSystem.h"

#include <exception>


IndexBuffer::IndexBuffer(void* list_indices, UINT size_list, RenderSystem* system) : m_system(system), m_buffer(0)
{
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = size_list * 4;
	buff_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_indices;

	m_size_list = size_list;

	HRESULT hr = m_system->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer);
	if (FAILED(hr))
	{
		throw std::exception("IndexBuffer initialization failed.");
	}
}

UINT IndexBuffer::getSizeIndexList()
{
	return this->m_size_list;
}

IndexBuffer::~IndexBuffer()
{
	m_buffer->Release();
}
