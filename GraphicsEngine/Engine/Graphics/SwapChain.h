#pragma once
#include <d3d11.h>

class DeviceContext;

class SwapChain
{
public:
	SwapChain();
	~SwapChain();

	bool Init(HWND hwnd, UINT width, UINT height);
	bool Present(bool vsync);
	bool Release();

private:
	IDXGISwapChain* m_SwapChain;
	ID3D11RenderTargetView* m_RenderTargetView;

	friend class DeviceContext;
};

