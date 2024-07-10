#include "ConstantBuffer.h"

#include <exception>
#include "DeviceContext.h"
#include "Engine/Renderer/RenderSystem.h"

ConstantBuffer::ConstantBuffer(void* buffer, UINT size_buffer, RenderSystem* system) : m_System(system)
{
	// Describes a buffer resource
	D3D11_BUFFER_DESC buffer_Desc = {};
	buffer_Desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_Desc.ByteWidth = size_buffer;
	buffer_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_Desc.CPUAccessFlags = 0;
	buffer_Desc.MiscFlags = 0;

	// Specifies data for initializing a subresource
	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = buffer;

	// Created Vertex Buffer
	if (FAILED(m_System->m_D3DDevice->CreateBuffer(&buffer_Desc, &init_data, &m_Buffer)))
	{
		throw std::exception("ERROR::Constant Buffer - Creation Failed");
	}
}

ConstantBuffer::~ConstantBuffer()
{
	if (m_Buffer) m_Buffer->Release();
}

void ConstantBuffer::Update(DeviceContext* context, void* buffer)
{
	context->m_DeviceContext->UpdateSubresource(this->m_Buffer, NULL, NULL, buffer, NULL, NULL);
}