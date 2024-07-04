#pragma once
#include <d3d11.h>

class SwapChain;
class DeviceContext;
class VertexBuffer;
class ConstantBuffer;
class IndexBuffer;
class VertexShader;
class PixelShader;

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();

	bool Init();		// Initialize GraphicsEngine and DirectX 11 Device
	bool Release();		// Release all resources loaded

	DeviceContext*	GetImmediateDeviceContext();
	SwapChain*		CreateSwapChain();

	VertexBuffer*	CreateVertexBuffer();
	IndexBuffer*	CreateIndexBuffer();
	ConstantBuffer* CreateConstantBuffer();

	VertexShader*	CreateVertexShader(const void* shader_byte_code, size_t byte_Code_Size);
	PixelShader*	CreatePixelShader(const void* shader_byte_code, size_t byte_Code_Size);

	bool CompileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	bool CompilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	void ReleaseCompiledShaders();

	static GraphicsEngine* Get();

private:
	DeviceContext* m_IMMDeviceContext;

	ID3D11Device* m_D3DDevice;
	D3D_FEATURE_LEVEL m_FeatureLevel;

	IDXGIDevice*  m_DXGIDevice;
	IDXGIAdapter* m_DXGIAdapter;
	IDXGIFactory* m_DXGIFactory;
	ID3D11DeviceContext* m_IMMContext;

	ID3DBlob* m_Blob = nullptr;

	// Temp
	ID3DBlob* m_vsblob = nullptr;
	ID3DBlob* m_psblob = nullptr;
	ID3D11PixelShader* m_ps = nullptr;

	friend class SwapChain;
	friend class VertexBuffer;
	friend class IndexBuffer;
	friend class ConstantBuffer;

	friend class VertexShader;
	friend class PixelShader;
};