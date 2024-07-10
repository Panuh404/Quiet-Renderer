#include <d3dcompiler.h>
#include <fstream>
#include <iostream>

#include "RenderSystem.h"
#include "Engine/Graphics/ConstantBuffer.h"
#include "Engine/Graphics/SwapChain.h"
#include "Engine/Graphics/VertexBuffer.h"
#include "Engine/Graphics/VertexShader.h"
#include "Engine/Graphics/DeviceContext.h"
#include "Engine/Graphics/IndexBuffer.h"
#include "Engine/Graphics/PixelShader.h"

RenderSystem::RenderSystem() {}

RenderSystem::~RenderSystem() {}

bool RenderSystem::Init()
{
	D3D_DRIVER_TYPE driver_types[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] = {
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	HRESULT res = 0;
	for (UINT driver_type_index = 0; driver_type_index < num_driver_types; )
	{
		res = D3D11CreateDevice(
			NULL,
			driver_types[driver_type_index],
			NULL,
			NULL,
			feature_levels,
			num_feature_levels,
			D3D11_SDK_VERSION,
			&m_D3DDevice,
			&m_FeatureLevel,
			&m_IMMContext);

		if (SUCCEEDED(res)) break;

		++driver_type_index;
	}
	if (FAILED(res))
	{
		return false;
	}

	m_IMMDeviceContext = new DeviceContext(m_IMMContext, this);

	m_D3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_DXGIDevice);
	m_DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_DXGIAdapter);
	m_DXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_DXGIFactory);

	return true;
}

bool RenderSystem::Release()
{
	m_DXGIDevice->Release();
	m_DXGIAdapter->Release();
	m_DXGIFactory->Release();

	delete m_IMMDeviceContext;
	
	m_D3DDevice->Release();
	return true;
}

SwapChain* RenderSystem::CreateSwapChain(HWND hwnd, UINT width, UINT height)
{
	SwapChain* sc = nullptr;
	try
	{
		sc = new SwapChain(hwnd, width, height, this);
	}
	catch (...) {}
	return sc;
}

DeviceContext* RenderSystem::GetImmediateDeviceContext()
{
	return this->m_IMMDeviceContext;
}

VertexBuffer* RenderSystem::CreateVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader)
{
	VertexBuffer* vb = nullptr;
	try
	{
		vb = new VertexBuffer(list_vertices, size_vertex, size_list, shader_byte_code, size_byte_shader, this);
	}
	catch (...)	{}
	return vb;
}

IndexBuffer* RenderSystem::CreateIndexBuffer(void* list_indices, UINT size_list)
{
	IndexBuffer* ib = nullptr;
	try
	{
		ib = new IndexBuffer(list_indices, size_list, this);
	}
	catch (...) {}
	return ib;
}

ConstantBuffer* RenderSystem::CreateConstantBuffer(void* buffer, UINT size_buffer)
{
	ConstantBuffer* cb = nullptr;
	try
	{
		cb = new ConstantBuffer(buffer, size_buffer, this);
	}
	catch (...) {}
	return cb;
}

VertexShader* RenderSystem::CreateVertexShader(const void* shader_byte_code, size_t byte_Code_Size)
{
	VertexShader* vs = nullptr;
	try
	{
		vs = new VertexShader(shader_byte_code, byte_Code_Size, this);
	}
	catch (...) {}
	return vs;
}

PixelShader* RenderSystem::CreatePixelShader(const void* shader_byte_code, size_t byte_Code_Size)
{
	PixelShader* ps = nullptr;
	try
	{
		ps = new PixelShader(shader_byte_code, byte_Code_Size, this);
	}
	catch (...) {}
	return ps;
}

bool RenderSystem::CompileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* errblob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &m_Blob, &errblob)))
	{
		if (errblob) errblob->Release();
		return false;
	}

	*shader_byte_code = m_Blob->GetBufferPointer();
	*byte_code_size = m_Blob->GetBufferSize();

	return true;
}

bool RenderSystem::CompilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* errblob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &m_Blob, &errblob)))
	{
		if (errblob) errblob->Release();
		return false;
	}

	*shader_byte_code = m_Blob->GetBufferPointer();
	*byte_code_size = m_Blob->GetBufferSize();

	return true;
}

void RenderSystem::ReleaseCompiledShaders()
{
	if (m_Blob) m_Blob->Release();
}
