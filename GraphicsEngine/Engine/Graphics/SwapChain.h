#pragma once
#include <d3d11.h>
#include "Engine/Prerequisites.h"

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system);
	~SwapChain();

	bool Present(bool vsync);

private:
	IDXGISwapChain* m_SwapChain;
	ID3D11RenderTargetView* m_RenderTargetView;
	RenderSystem* m_System = nullptr;

	friend class DeviceContext;
};

