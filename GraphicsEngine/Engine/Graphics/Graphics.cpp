#include "Graphics.h"
#include "SwapChain.h"
#include "DeviceContext.h"

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

bool Graphics::Init()
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
	ID3D11DeviceContext* m_IMMContext;
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

bool Graphics::Release()
{
	m_DXGIDevice->Release();
	m_DXGIAdapter->Release();
	m_DXGIFactory->Release();

	m_IMMDeviceContext->Release();
	m_D3DDevice->Release();
	return true;
}

SwapChain* Graphics::CreateSwapChain()
{
	return new SwapChain();
}

DeviceContext* Graphics::GetImmediateDeviceContext()
{
	return this->m_IMMDeviceContext;
}

Graphics* Graphics::Get()
{
	static Graphics engine;
	return &engine;
}
