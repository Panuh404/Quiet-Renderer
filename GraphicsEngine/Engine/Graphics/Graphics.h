#pragma once
#include <d3d11.h>

class SwapChain;
class DeviceContext;

class Graphics
{
public:
	Graphics();
	~Graphics();

	bool Init();		// Initialize Graphics and DirectX 11 Device
	bool Release();		// Release all resources loaded

	SwapChain* CreateSwapChain();
	DeviceContext* GetImmediateDeviceContext();

	static Graphics* Get();

private:
	ID3D11Device* m_D3DDevice;
	D3D_FEATURE_LEVEL m_FeatureLevel;

	IDXGIDevice*  m_DXGIDevice;
	IDXGIAdapter* m_DXGIAdapter;
	IDXGIFactory* m_DXGIFactory;

	DeviceContext* m_IMMDeviceContext;

	friend class SwapChain;
};

