#include "VertexBuffer.h"
#include "GraphicsEngine.h"

VertexBuffer::VertexBuffer() : m_Layout(0), m_Buffer(0) {}

VertexBuffer::~VertexBuffer() {}

bool VertexBuffer::Load(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader)
{
	if (m_Buffer) m_Buffer->Release();
	if (m_Layout) m_Layout->Release();


	// Describes a buffer resource
	D3D11_BUFFER_DESC buffer_Desc = {};
	buffer_Desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_Desc.ByteWidth = size_vertex * size_list;
	buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_Desc.CPUAccessFlags = 0;
	buffer_Desc.MiscFlags = 0;

	// Specifies data for initializing a subresource
	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_vertices;

	m_SizeVertex = size_vertex;
	m_SizeList = size_list;

	// Created Vertex Buffer
	if (FAILED(GraphicsEngine::Get()->m_D3DDevice->CreateBuffer(&buffer_Desc, &init_data, &m_Buffer)))
	{
		return false;
	}

	// A description of a single element for the input-assembler stage.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	UINT size_layout = ARRAYSIZE(layout);

	// Create an input-layout object to describe the input-buffer data for the input-assembler stage
	if (FAILED(GraphicsEngine::Get()->m_D3DDevice->CreateInputLayout(layout, size_layout, shader_byte_code, size_byte_shader, &m_Layout)))
	{
		return false;
	}
	
	return true;
}

bool VertexBuffer::Release()
{
	m_Layout->Release();
	m_Buffer->Release();
	delete this;
	return false;
}

UINT VertexBuffer::GetSizeVertexList()
{
	return this->m_SizeList;
}