#include "IndexBuffer.h"

#include <exception>
#include "Engine/Renderer/RenderSystem.h"

IndexBuffer::IndexBuffer(void* list_indices, UINT size_list, RenderSystem* system) : m_System(system), m_Buffer(0)
{
	// Describes a buffer resource
	D3D11_BUFFER_DESC buffer_Desc = {};
	buffer_Desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_Desc.ByteWidth = 4 * size_list;
	buffer_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffer_Desc.CPUAccessFlags = 0;
	buffer_Desc.MiscFlags = 0;

	// Specifies data for initializing a subresource
	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_indices;

	m_SizeList = size_list;

	// Created Vertex Buffer
	if (FAILED(m_System->m_D3DDevice->CreateBuffer(&buffer_Desc, &init_data, &m_Buffer)))
	{
		throw std::exception("ERROR::Index Buffer - Creation Failed");
	}
}

IndexBuffer::~IndexBuffer()
{
	m_Buffer->Release();
}

UINT IndexBuffer::GetSizeIndexList()
{
	return this->m_SizeList;
}
