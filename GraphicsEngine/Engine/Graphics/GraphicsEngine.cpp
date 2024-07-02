#include <d3dcompiler.h>
#include <fstream>
#include "GraphicsEngine.h"

#include <iostream>

#include "SwapChain.h"
#include "VertexBuffer.h"
#include "DeviceContext.h"

GraphicsEngine::GraphicsEngine()
{
}

GraphicsEngine::~GraphicsEngine()
{
}

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

bool GraphicsEngine::CreateShaders()
{
	LPCWSTR SourceFile = L"Shaders/shader.fx";
	ID3DBlob* errblob = nullptr;
	D3DCompileFromFile(SourceFile, nullptr, nullptr, "vsmain", "vs_5_0", NULL, NULL, &m_vsblob, &errblob);
	D3DCompileFromFile(SourceFile, nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &m_psblob, &errblob);
	m_D3DDevice->CreateVertexShader(m_vsblob->GetBufferPointer(), m_vsblob->GetBufferSize(), nullptr, &m_vs);
	m_D3DDevice->CreatePixelShader(m_psblob->GetBufferPointer(), m_psblob->GetBufferSize(), nullptr, &m_ps);
	return true;
}

bool GraphicsEngine::SetShaders()
{
	m_IMMContext->VSSetShader(m_vs, nullptr, 0);
	m_IMMContext->PSSetShader(m_ps, nullptr, 0);
	return true;
}

void GraphicsEngine::GetShaderBufferAndSize(void** bytecode, UINT* size)
{
	*bytecode = this->m_vsblob->GetBufferPointer();
	*size = (UINT)this->m_vsblob->GetBufferSize();
}

GraphicsEngine* GraphicsEngine::Get()
{
	static GraphicsEngine engine;
	return &engine;
}
