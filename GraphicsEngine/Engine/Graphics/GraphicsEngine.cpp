#include <d3dcompiler.h>
#include <fstream>
#include <iostream>

#include "GraphicsEngine.h"
#include "ConstantBuffer.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "DeviceContext.h"
#include "PixelShader.h"

GraphicsEngine::GraphicsEngine() {}

GraphicsEngine::~GraphicsEngine() {}

bool GraphicsEngine::Init()
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

	m_IMMDeviceContext = new DeviceContext(m_IMMContext);

	m_D3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_DXGIDevice);
	m_DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_DXGIAdapter);
	m_DXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_DXGIFactory);

	return true;
}

bool GraphicsEngine::Release()
{
	m_DXGIDevice->Release();
	m_DXGIAdapter->Release();
	m_DXGIFactory->Release();

	m_IMMDeviceContext->Release();
	m_D3DDevice->Release();
	return true;
}

SwapChain* GraphicsEngine::CreateSwapChain()
{
	return new SwapChain();
}

DeviceContext* GraphicsEngine::GetImmediateDeviceContext()
{
	return this->m_IMMDeviceContext;
}

VertexBuffer* GraphicsEngine::CreateVertexBuffer()
{
	return new VertexBuffer();
}

ConstantBuffer* GraphicsEngine::CreateConstantBuffer()
{
	return new ConstantBuffer();
}

VertexShader* GraphicsEngine::CreateVertexShader(const void* shader_byte_code, size_t byte_Code_Size)
{
	VertexShader* vs_shader = new VertexShader();

	if (!vs_shader->Init(shader_byte_code, byte_Code_Size))
	{
		vs_shader->Release();
		return nullptr;
	}
	return vs_shader;
}

PixelShader* GraphicsEngine::CreatePixelShader(const void* shader_byte_code, size_t byte_Code_Size)
{
	PixelShader* ps_shader = new PixelShader();

	if (!ps_shader->Init(shader_byte_code, byte_Code_Size))
	{
		ps_shader->Release();
		return nullptr;
	}
	return ps_shader;
}

bool GraphicsEngine::CompileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
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

bool GraphicsEngine::CompilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
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

void GraphicsEngine::ReleaseCompiledShaders()
{
	if (m_Blob) m_Blob->Release();
}

GraphicsEngine* GraphicsEngine::Get()
{
	static GraphicsEngine engine;
	return &engine;
}
