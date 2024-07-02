#pragma once
#include <d3d11.h>

class SwapChain;
class DeviceContext;
class VertexBuffer;

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();

	bool Init();		// Initialize GraphicsEngine and DirectX 11 Device
	bool Release();		// Release all resources loaded

	SwapChain*		CreateSwapChain();
	DeviceContext*	GetImmediateDeviceContext();
	VertexBuffer*	CreateVertexBuffer();

	// TEMP
	bool CreateShaders();
	bool SetShaders();
	void GetShaderBufferAndSize(void** bytecode, UINT* size);

	static GraphicsEngine* Get();

private:
	DeviceContext* m_IMMDeviceContext;

	ID3D11Device* m_D3DDevice;
	D3D_FEATURE_LEVEL m_FeatureLevel;

	IDXGIDevice*  m_DXGIDevice;
	IDXGIAdapter* m_DXGIAdapter;
	IDXGIFactory* m_DXGIFactory;
	ID3D11DeviceContext* m_IMMContext;

	// Temp
	ID3DBlob* m_vsblob = nullptr;
	ID3DBlob* m_psblob = nullptr;
	ID3D11VertexShader* m_vs = nullptr;
	ID3D11PixelShader* m_ps = nullptr;

	friend class SwapChain;
	friend class VertexBuffer;
};

